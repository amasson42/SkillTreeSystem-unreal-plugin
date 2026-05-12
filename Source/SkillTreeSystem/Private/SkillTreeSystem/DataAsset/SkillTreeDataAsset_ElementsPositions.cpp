
#include "SkillTreeSystem/DataAsset/SkillTreeDataAsset.h"


// SkillTreeDataAsset.cpp

void USkillTreeDataAsset::GetCanvasElementsPositions_Implementation(
    TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
    TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
    // ------------------------------------------------------------------
    // 0. Resolve effective layout per node, guard against Default→Default
    // ------------------------------------------------------------------

    // DefaultLayout.Method == Default is a misconfiguration — treat as Absolute at origin
    const bool bDefaultIsSafe = DefaultLayout.Method != ESkillTreeDataAssetLayoutMethod::Default;

    auto GetEffectiveLayout = [&](const FSkillTreeDataAssetElement& Element)
        -> const FSkillTreeDataAssetLayoutData&
    {
        if (Element.Layout.Method != ESkillTreeDataAssetLayoutMethod::Default)
            return Element.Layout;

        // Falls through to DefaultLayout — safe only if it isn't also Default
        if (bDefaultIsSafe)
            return DefaultLayout;

        // Misconfigured: return the node's own layout data (which is Default, a no-op)
        // The node will simply be skipped below
        return Element.Layout;
    };

    // ------------------------------------------------------------------
    // 1. Bucket nodes by their effective method
    // ------------------------------------------------------------------
    TArray<FGameplayTag> ExternalNodes;
    TArray<FGameplayTag> AbsoluteNodes;
    TArray<FGameplayTag> RelativeNodes;

    for (const auto& [NodeId, NodeData] : Nodes)
    {
        switch (GetEffectiveLayout(NodeData).Method)
        {
        case ESkillTreeDataAssetLayoutMethod::External:  ExternalNodes.Add(NodeId); break;
        case ESkillTreeDataAssetLayoutMethod::Absolute:  AbsoluteNodes.Add(NodeId); break;
        case ESkillTreeDataAssetLayoutMethod::Relative:  RelativeNodes.Add(NodeId); break;
        default: /* Default→Default misconfiguration, skip */ break;
        }
    }

    // ------------------------------------------------------------------
    // 2. External nodes — delegate to ExternalLayoutSource
    //    Only call once, let it fill whatever it knows about
    // ------------------------------------------------------------------
    if (ExternalNodes.Num() > 0
        && ExternalLayoutSource
        && IsValid(ExternalLayoutSource.GetObject())
        && ExternalLayoutSource.GetObject() != this) // infinite loop guard
    {
        ISkillTreeLayoutMaker::Execute_GetCanvasElementsPositions(
            ExternalLayoutSource.GetObject(),
            OutNodePositions, OutLinkPositions);

        // Remove any External node that the source didn't fill
        // (we won't try to re-position them — they'll simply be absent)
    }

    // ------------------------------------------------------------------
    // 3. Absolute nodes — direct placement, no dependencies
    // ------------------------------------------------------------------
    for (const FGameplayTag& NodeId : AbsoluteNodes)
    {
        const FSkillTreeDataAssetElement* NodeData = Nodes.Find(NodeId);
        if (!NodeData) continue;

        const FSkillTreeDataAssetLayoutData& Layout = GetEffectiveLayout(*NodeData);

        FSkillTreeNodePosition Pos;
        Pos.CanvasLocation = Layout.AbsolutePosition;
        OutNodePositions.Add(NodeId, Pos);
    }

    // ------------------------------------------------------------------
    // 4. Relative nodes — topological sort (Kahn's algorithm)
    //    Parents may be any method — we only need their position resolved first
    // ------------------------------------------------------------------
    if (RelativeNodes.Num() > 0)
    {
        _ResolveRelativeNodes(RelativeNodes, GetEffectiveLayout, OutNodePositions);
    }

    // ------------------------------------------------------------------
    // 5. Populate all link positions from final resolved node positions
    // ------------------------------------------------------------------
    _PopulateLinksFromPositions(OutNodePositions, OutLinkPositions);
}

// Internal helper — resolves the angle the AutoSpread arc is centered on.
// ParentOrigin is the blended parent position already computed for this node.
static float _ResolveSpreadCenterAngle(
    const FSkillTreeDataAssetRelativeLayoutData& Rel,
    const FVector2D& ParentOrigin)
{
    switch (Rel.SpreadDirection)
    {
    case ESkillTreeDataAssetLayoutSpreadDirection::Down:
        return 90.0f;

    case ESkillTreeDataAssetLayoutSpreadDirection::Up:
        return 270.0f;

    case ESkillTreeDataAssetLayoutSpreadDirection::Right:
        return 0.0f;

    case ESkillTreeDataAssetLayoutSpreadDirection::Left:
        return 180.0f;

    case ESkillTreeDataAssetLayoutSpreadDirection::CustomAngle:
        return Rel.SpreadCustomAngleDeg;

    case ESkillTreeDataAssetLayoutSpreadDirection::FromCenter:
        {
            // Direction from the (offset) center point toward the parent's blended origin
            // — children will fan out continuing away from center
            const FVector2D EffectiveCenter = Rel.SpreadCenterPoint + Rel.SpreadCenterOffset;
            const FVector2D Dir = ParentOrigin - EffectiveCenter;
            if (Dir.IsNearlyZero())
                return 90.0f; // center coincides with parent — fall back to down
            return FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X));
        }
    }

    return 90.0f;
}

void USkillTreeDataAsset::_ResolveRelativeNodes(
    const TArray<FGameplayTag>& RelativeNodes,
    TFunctionRef<const FSkillTreeDataAssetLayoutData&(const FSkillTreeDataAssetElement&)> GetEffectiveLayout,
    TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions)
{
    // Build a set for O(1) membership checks
    TSet<FGameplayTag> RelativeSet(RelativeNodes);

    // In-degree counts only unresolved Relative parents
    // (Absolute/External parents are already in OutNodePositions)
    TMap<FGameplayTag, int32>              InDegree;
    TMap<FGameplayTag, TArray<FGameplayTag>> Dependents; // RelativeParent -> its Relative children

    for (const FGameplayTag& NodeId : RelativeNodes)
    {
        const FSkillTreeDataAssetElement* NodeData = Nodes.Find(NodeId);
        if (!NodeData) continue;

        int32 UnresolvedCount = 0;
        for (const auto& [ParentId, _] : NodeData->ParentLinks)
        {
            if (!RelativeSet.Contains(ParentId)) continue; // already resolved or external
            ++UnresolvedCount;
            Dependents.FindOrAdd(ParentId).Add(NodeId);
        }
        InDegree.Add(NodeId, UnresolvedCount);
    }

    // Pre-compute sibling groups per parent for AutoSpread angle distribution
    // Maps ParentId -> ordered list of Relative children under that parent
    TMap<FGameplayTag, TArray<FGameplayTag>> SiblingGroups;
    for (const FGameplayTag& NodeId : RelativeNodes)
    {
        const FSkillTreeDataAssetElement* NodeData = Nodes.Find(NodeId);
        if (!NodeData) continue;
        for (const auto& [ParentId, _] : NodeData->ParentLinks)
            SiblingGroups.FindOrAdd(ParentId).AddUnique(NodeId);
    }

    // Seed: Relative nodes with all parents already resolved
    TArray<FGameplayTag> Queue;
    for (const FGameplayTag& NodeId : RelativeNodes)
    {
        // Also skip nodes whose non-Relative parents weren't resolved by External/Absolute
        // — they'd have no valid origin to compute from
        bool bAllParentsResolved = true;
        const FSkillTreeDataAssetElement* NodeData = Nodes.Find(NodeId);
        if (NodeData)
        {
            for (const auto& [ParentId, _] : NodeData->ParentLinks)
            {
                if (!OutNodePositions.Contains(ParentId) && !RelativeSet.Contains(ParentId))
                {
                    bAllParentsResolved = false;
                    break;
                }
            }
        }

        if (InDegree[NodeId] == 0 && bAllParentsResolved)
            Queue.Add(NodeId);
    }

    while (Queue.Num() > 0)
    {
        const FGameplayTag NodeId = Queue.Pop(EAllowShrinking::No);

        const FSkillTreeDataAssetElement* NodeData = Nodes.Find(NodeId);
        if (!NodeData) continue;

        const FSkillTreeDataAssetRelativeLayoutData& Rel =
            GetEffectiveLayout(*NodeData).Relative;

        // ----------------------------------------------------------
        // Gather resolved parent positions
        // ----------------------------------------------------------
        TArray<FVector2D> ParentPositions;
        for (const auto& [ParentId, _] : NodeData->ParentLinks)
        {
            if (const FSkillTreeNodePosition* P = OutNodePositions.Find(ParentId))
                ParentPositions.Add(P->CanvasLocation);
        }

        if (ParentPositions.Num() == 0)
        {
            // No resolved parents at all — place at origin as fallback
            FSkillTreeNodePosition Fallback;
            Fallback.CanvasLocation = Rel.ManualOffset;
            OutNodePositions.Add(NodeId, Fallback);
            continue;
        }

        // ----------------------------------------------------------
        // Blend parent positions into a single origin
        // 0.0 = first parent only, 1.0 = last parent only, 0.5 = centroid
        // ----------------------------------------------------------
        FVector2D Origin = FVector2D::ZeroVector;
        if (ParentPositions.Num() == 1)
        {
            Origin = ParentPositions[0];
        }
        else
        {
            // Weighted lerp chain: t=0 → first, t=1 → last, t=0.5 → equal weight
            const float t = Rel.MultiParentBlend;
            FVector2D Accumulated = ParentPositions[0];
            for (int32 i = 1; i < ParentPositions.Num(); ++i)
            {
                const float Weight = t * static_cast<float>(i)
                    / static_cast<float>(ParentPositions.Num() - 1);
                Accumulated = FMath::Lerp(Accumulated, ParentPositions[i], Weight);
            }
            Origin = Accumulated;
        }

        // ----------------------------------------------------------
        // Resolve angle
        // ----------------------------------------------------------
        float AngleDeg = 90.0f; // fallback: downward

        switch (Rel.AngleMode)
        {
        case ESkillTreeDataAssetLayoutRelativeAngleMode::Explicit:
            {
                AngleDeg = Rel.ExplicitAngleDeg;
                break;
            }

        case ESkillTreeDataAssetLayoutRelativeAngleMode::Inherited:
            {
                // Direction from first parent's own first parent → first parent
                const FGameplayTag& FirstParentId = NodeData->ParentLinks.begin()->Key;
                if (const FSkillTreeDataAssetElement* ParentElem = Nodes.Find(FirstParentId))
                {
                    if (ParentElem->ParentLinks.Num() > 0)
                    {
                        const FGameplayTag& GrandparentId = ParentElem->ParentLinks.begin()->Key;
                        if (const FSkillTreeNodePosition* GrandparentPos = OutNodePositions.Find(GrandparentId))
                        {
                            const FVector2D Dir = ParentPositions[0] - GrandparentPos->CanvasLocation;
                            if (!Dir.IsNearlyZero())
                                AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X));
                            // else keep fallback — grandparent is coincident with parent
                        }
                        // else grandparent not yet resolved — keep fallback
                    }
                    // else parent is a root — no inherited direction, keep fallback
                }
                break;
            }

        case ESkillTreeDataAssetLayoutRelativeAngleMode::AutoSpread:
            {
                const FGameplayTag& FirstParentId = NodeData->ParentLinks.begin()->Key;
                if (const TArray<FGameplayTag>* Siblings = SiblingGroups.Find(FirstParentId))
                {
                    const int32 Count = Siblings->Num();
                    if (Count > 1)
                    {
                        const int32 Index       = Siblings->IndexOfByKey(NodeId);
                        const float HalfArc     = Rel.SpreadArcDeg * 0.5f;
                        const float CenterAngle = _ResolveSpreadCenterAngle(Rel, Origin);

                        AngleDeg = CenterAngle - HalfArc
                            + Rel.SpreadArcDeg * static_cast<float>(Index)
                            / static_cast<float>(Count - 1);
                    }
                    else
                    {
                        // Single child — place it exactly on the center angle, no spread needed
                        AngleDeg = _ResolveSpreadCenterAngle(Rel, Origin);
                    }
                }
                break;
            }
        }

        // ----------------------------------------------------------
        // Compute final position
        // ----------------------------------------------------------
        const float AngleRad  = FMath::DegreesToRadians(AngleDeg);
        FVector2D   FinalPos  = Origin
            + FVector2D(FMath::Cos(AngleRad), FMath::Sin(AngleRad)) * Rel.Distance
            + Rel.ManualOffset;

        if (Rel.GridSnap > KINDA_SMALL_NUMBER)
        {
            FinalPos.X = FMath::RoundToFloat(FinalPos.X / Rel.GridSnap) * Rel.GridSnap;
            FinalPos.Y = FMath::RoundToFloat(FinalPos.Y / Rel.GridSnap) * Rel.GridSnap;
        }

        FSkillTreeNodePosition NodePos;
        NodePos.CanvasLocation = FinalPos;
        OutNodePositions.Add(NodeId, NodePos);

        // Unblock dependent Relative children
        if (const TArray<FGameplayTag>* Children = Dependents.Find(NodeId))
        {
            for (const FGameplayTag& ChildId : *Children)
            {
                int32& Deg = InDegree.FindChecked(ChildId);
                if (--Deg == 0)
                    Queue.Add(ChildId);
            }
        }
    }
}

void USkillTreeDataAsset::_PopulateLinksFromPositions(
    const TMap<FGameplayTag, FSkillTreeNodePosition>& NodePositions,
    TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
    for (const auto& [NodeId, NodeData] : Nodes)
    {
        const FSkillTreeNodePosition* ChildPos = NodePositions.Find(NodeId);
        if (!ChildPos) continue;

        for (const auto& [ParentId, _] : NodeData.ParentLinks)
        {
            const FSkillTreeNodePosition* ParentPos = NodePositions.Find(ParentId);
            if (!ParentPos) continue;

            // Don't overwrite a link already populated by ExternalLayoutSource
            // (it may have richer curve data we don't want to clobber)
            const FSkillTreeLinkName LinkName {ParentId, NodeId};
            if (OutLinkPositions.Contains(LinkName)) continue;

            FSkillTreeLinkPosition LinkPos;
            LinkPos.CanvasStartLocation = ParentPos->CanvasLocation;
            LinkPos.CanvasEndLocation   = ChildPos->CanvasLocation;
            OutLinkPositions.Add(LinkName, LinkPos);
        }
    }
}
