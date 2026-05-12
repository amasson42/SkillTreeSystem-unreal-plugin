// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/CanvasSource/LayoutMaker/SkillTreeHierarchicalLayoutMaker.h"


// ---------------------------------------------------------------------------
// Internal node representation — local to this translation unit
// ---------------------------------------------------------------------------

struct _HierNodeData
{
    FGameplayTag        Name;
    _HierNodeData*          Parent      = nullptr;
    TArray<_HierNodeData*>  Children;
    int32               Depth       = 0;

    // Reingold-Tilford intermediate values
    float               Prelim      = 0.f;   // preliminary x before shift
    float               Mod         = 0.f;   // modifier propagated to subtree
    float               Shift       = 0.f;   // shift applied to whole subtree
    float               Change      = 0.f;   // change applied while walking

    // Reingold-Tilford thread pointer (for contour traversal)
    _HierNodeData*          Thread      = nullptr;
    _HierNodeData*          Ancestor    = nullptr; // used during separation step

    int32               ChildIndex  = 0;       // position among siblings
    FVector2D           FinalPos    = FVector2D::ZeroVector;

    explicit _HierNodeData(FGameplayTag InName) : Name(InName), Ancestor(this) {}
};

// ---------------------------------------------------------------------------
// Reingold-Tilford helpers
// ---------------------------------------------------------------------------

static _HierNodeData* _Leftmost(_HierNodeData* Node)
{
    return Node->Thread ? Node->Thread
         : Node->Children.Num() > 0 ? Node->Children[0]
         : nullptr;
}

static _HierNodeData* _Rightmost(_HierNodeData* Node)
{
    return Node->Thread ? Node->Thread
         : Node->Children.Num() > 0 ? Node->Children.Last()
         : nullptr;
}

static void _MoveSubtree(_HierNodeData* WL, _HierNodeData* WR, float Shift)
{
    const int32 SubtreeCount = WR->ChildIndex - WL->ChildIndex;
    if (SubtreeCount <= 0) return;

    WR->Change -= Shift / SubtreeCount;
    WR->Shift  += Shift;
    WL->Change += Shift / SubtreeCount;
    WR->Prelim += Shift;
    WR->Mod    += Shift;
}

static _HierNodeData* _Apportion(_HierNodeData* V, _HierNodeData* DefaultAncestor, float SiblingSpacing)
{
    if (V->ChildIndex == 0) return DefaultAncestor;

    _HierNodeData* VR = V;                              // right contour of left siblings
    _HierNodeData* VL = V->Parent->Children[V->ChildIndex - 1]; // left sibling
    _HierNodeData* SR = V;                              // left contour of V
    _HierNodeData* SL = VL;

    float ModVR = VR->Mod;
    float ModVL = VL->Mod;
    float ModSR = SR->Mod;
    float ModSL = SL->Mod;

    while (_Rightmost(VL) && _Leftmost(VR))
    {
        VL = _Rightmost(VL);
        VR = _Leftmost(VR);

        if (VL->Parent) SL = _Leftmost(VL->Parent->Children[0]);
        SR = VR; // simplification: walk right contour of V

        const float Shift = (VL->Prelim + ModVL) - (VR->Prelim + ModVR) + SiblingSpacing;
        if (Shift > 0.f)
        {
            _HierNodeData* AncestorNode = (VL->Ancestor && VL->Ancestor->Parent == V->Parent)
                ? VL->Ancestor
                : DefaultAncestor;

            _MoveSubtree(AncestorNode, V, Shift);
            ModVR += Shift;
            ModSR += Shift;
        }

        ModVL += VL->Mod;
        ModVR += VR->Mod;
        ModSL += SL ? SL->Mod : 0.f;
        ModSR += SR->Mod;
    }

    // Set thread for contour continuation
    if (_Rightmost(VL) && !_Rightmost(SR))
    {
        SR->Thread = _Rightmost(VL);
        SR->Mod   += ModVL - ModSR;
    }
    if (_Leftmost(VR) && !_Leftmost(SL))
    {
        if (SL)
        {
            SL->Thread = _Leftmost(VR);
            SL->Mod   += ModVR - ModSL;
        }
        DefaultAncestor = V;
    }

    return DefaultAncestor;
}

// First walk: assign preliminary positions bottom-up
static void _FirstWalk(_HierNodeData* V, float SiblingSpacing)
{
    if (V->Children.Num() == 0)
    {
        // Leaf: space relative to left sibling
        if (V->ChildIndex > 0)
            V->Prelim = V->Parent->Children[V->ChildIndex - 1]->Prelim + SiblingSpacing;
        return;
    }

    _HierNodeData* DefaultAncestor = V->Children[0];

    for (_HierNodeData* Child : V->Children)
    {
        _FirstWalk(Child, SiblingSpacing);
        DefaultAncestor = _Apportion(Child, DefaultAncestor, SiblingSpacing);
    }

    // Execute shifts accumulated during apportion
    float ShiftAccum  = 0.f;
    float ChangeAccum = 0.f;
    for (int32 i = V->Children.Num() - 1; i >= 0; --i)
    {
        _HierNodeData* Child  = V->Children[i];
        Child->Prelim    += ShiftAccum;
        Child->Mod       += ShiftAccum;
        ChangeAccum      += Child->Change;
        ShiftAccum       += Child->Shift + ChangeAccum;
    }

    const float Midpoint = (V->Children[0]->Prelim + V->Children.Last()->Prelim) / 2.f;
    if (V->ChildIndex > 0 && V->Parent)
        V->Prelim = V->Parent->Children[V->ChildIndex - 1]->Prelim + SiblingSpacing;

    V->Mod = V->Prelim - Midpoint;
}

// Second walk: convert preliminary positions to absolute
static void _SecondWalk(_HierNodeData* V, float ModSum, float LevelSpacing)
{
    V->FinalPos.X = V->Prelim + ModSum;
    V->FinalPos.Y = static_cast<float>(V->Depth) * LevelSpacing;

    for (_HierNodeData* Child : V->Children)
        _SecondWalk(Child, ModSum + V->Mod, LevelSpacing);
}

// ---------------------------------------------------------------------------
// Output helpers
// ---------------------------------------------------------------------------

static FVector2D _ApplyDirection(
    FVector2D InPos,
    ESkillTreeHierarchicalLayoutDirection Direction)
{
    switch (Direction)
    {
    case ESkillTreeHierarchicalLayoutDirection::TopToBottom:
        return InPos;                                           // X = cross, Y = depth
    case ESkillTreeHierarchicalLayoutDirection::BottomToTop:
        return FVector2D(InPos.X, -InPos.Y);
    case ESkillTreeHierarchicalLayoutDirection::LeftToRight:
        return FVector2D(InPos.Y, InPos.X);                    // swap axes
    case ESkillTreeHierarchicalLayoutDirection::RightToLeft:
        return FVector2D(-InPos.Y, InPos.X);
    }
    return InPos;
}

static void _PopulateOutput(
    const TMap<FGameplayTag, _HierNodeData>& NodeMap,
    ESkillTreeHierarchicalLayoutDirection Direction,
    TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
    TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
    for (const auto& [Tag, Node] : NodeMap)
    {
        FSkillTreeNodePosition NodePos;
        NodePos.CanvasLocation = _ApplyDirection(Node.FinalPos, Direction);
        OutNodePositions.Add(Tag, NodePos);

        if (Node.Parent && Node.Parent->Name.IsValid())
        {
            FSkillTreeLinkPosition LinkPos;
            LinkPos.CanvasStartLocation = _ApplyDirection(Node.Parent->FinalPos, Direction);
            LinkPos.CanvasEndLocation   = NodePos.CanvasLocation;
            OutLinkPositions.Add({Node.Parent->Name, Tag}, LinkPos);
        }
    }
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

void USkillTreeHierarchicalLayoutMaker::GetCanvasElementsPositions_Implementation(
	TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
	// Build node map
	TMap<FGameplayTag, _HierNodeData> NodeMap;
	NodeMap.Reserve(Nodes.Num());
	for (const auto& Elem : Nodes)
		NodeMap.Emplace(Elem.Name, _HierNodeData{Elem.Name});

	// Wire parents, children, child indices, depths
	for (const auto& Elem : Nodes)
	{
		if (!Elem.Parent.IsValid()) continue;

		_HierNodeData* NodePtr   = NodeMap.Find(Elem.Name);
		_HierNodeData* ParentPtr = NodeMap.Find(Elem.Parent);
		if (!NodePtr || !ParentPtr) continue;

		NodePtr->Parent     = ParentPtr;
		NodePtr->ChildIndex = ParentPtr->Children.Num();
		ParentPtr->Children.Add(NodePtr);
	}

	// Assign depths via BFS from roots, collect roots
	TArray<_HierNodeData*> Roots;
	for (auto& [Tag, Node] : NodeMap)
	{
		if (!Node.Parent)
		{
			Node.Depth = 0;
			Roots.Add(&Node);
		}
	}

	// BFS depth assignment
	TArray<_HierNodeData*> Queue = Roots;
	for (int32 i = 0; i < Queue.Num(); ++i)
	{
		_HierNodeData* Current = Queue[i];
		for (_HierNodeData* Child : Current->Children)
		{
			Child->Depth = Current->Depth + 1;
			Queue.Add(Child);
		}
	}

	// If multiple roots, create a virtual root to unify the forest
	_HierNodeData VirtualRoot{FGameplayTag()};
	_HierNodeData* LayoutRoot;

	if (Roots.Num() == 1)
	{
		LayoutRoot = Roots[0];
	}
	else
	{
		// Shift all real nodes down one depth level
		for (auto& [Tag, Node] : NodeMap)
			Node.Depth += 1;

		VirtualRoot.Children = Roots;
		for (int32 i = 0; i < Roots.Num(); ++i)
		{
			Roots[i]->Parent     = &VirtualRoot;
			Roots[i]->ChildIndex = i;
		}
		LayoutRoot = &VirtualRoot;
	}

	// Run Reingold-Tilford
	_FirstWalk(LayoutRoot, SiblingSpacing);
	_SecondWalk(LayoutRoot, 0.f, LevelSpacing);

	_PopulateOutput(NodeMap, Direction, OutNodePositions, OutLinkPositions);
}