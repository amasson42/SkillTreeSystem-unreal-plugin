// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/DataAsset/SkillTreeDataAsset.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/StateController/SkillTreeStateControllerEditable.h"
#include "SkillTreeSystem/Widgets/SkillTreeNodeWidget.h"
#include "SkillTreeSystem/Widgets/SkillTreeLinkWidget.h"

void USkillTreeDataAsset::GetCanvasNodesIds_Implementation(TArray<FGameplayTag>& OutNodes)
{
	Nodes.GetKeys(OutNodes);
}

void USkillTreeDataAsset::GetCanvasNodeData_Implementation(const FGameplayTag& NodeId,
	TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass,
	FSkillTreeNodeAppearance& OutAppearance)
{
	if (const auto* Node = Nodes.Find(NodeId))
	{
		OutAppearance = Node->Appearance;
		if (IsValid(Node->NodeWidgetClass))
		{
			OutWidgetClass = Node->NodeWidgetClass;
			return;
		}
	}
	
	if (IsValid(DefaultNodeWidgetClass))
	{
		OutWidgetClass = DefaultNodeWidgetClass;
		return;
	}
}

void USkillTreeDataAsset::GetCanvasLinkIds_Implementation(TArray<FSkillTreeLinkName>& OutLinks)
{
	for (const auto& [NodeName, NodeData] : Nodes)
	{
		for (const auto& [ParentName, ParentData] : NodeData.ParentLinks)
		{
			OutLinks.Add({ParentName, NodeName});
		}
	}
}

void USkillTreeDataAsset::GetCanvasLinkData_Implementation(
	const FSkillTreeLinkName& LinkName,
	TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass,
	FSkillTreeLinkAppearance& OutLinkAppearance)
{
	if (const auto* Node = Nodes.Find(LinkName.StartNodeName))
	{
		if (const auto* ParentLink = Node->ParentLinks.Find(LinkName.EndNodeName))
		{
			OutLinkAppearance = ParentLink->LinkAppearance;
			if (IsValid(ParentLink->LinkWidgetClass))
			{
				OutWidgetClass = ParentLink->LinkWidgetClass;
				return;
			}
		}
	}
	
	if (IsValid(DefaultLinkWidgetClass))
	{
		OutWidgetClass = DefaultLinkWidgetClass;
		OutLinkAppearance = DefaultLinkAppearance;
	}
}

void USkillTreeDataAsset::GetNodesIds_Implementation(TArray<FGameplayTag>& OutNodes) const
{
	Nodes.GetKeys(OutNodes);
}

bool USkillTreeDataAsset::CanUpgradeNode_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const
{
	const auto* Node = Nodes.Find(NodeId);
	if (!Node) return false;
	
	FSkillTreeNodeState NodeState;
	State->GetNodeState(NodeId, NodeState);
	return FSkillTreeLevelRequirement::CanStateUpgradeLevel(State, Node->Levels, GlobalRequirement, NodeState.Level);
}

void USkillTreeDataAsset::UpdateNodeState_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State)
{
	const auto* Node = Nodes.Find(NodeId);
	if (!Node) return;
	
	FSkillTreeNodeState NewNodeState;
	State->GetNodeState(NodeId, NewNodeState);
	
	bool bModified = false;
	
	auto EnsureStatus = [&bModified, &NewNodeState](const ESkillTreeElementStatus ExpectedStatus) {
		if (NewNodeState.Status != ExpectedStatus)
		{
			bModified = true;
			NewNodeState.Status = ExpectedStatus;
		}
	};
	
	if (NewNodeState.Level < 0)
	{
		NewNodeState.Level = 0;
		bModified = true;
	}
	
	if (NewNodeState.MaxLevel != Node->Levels.Num())
	{
		NewNodeState.MaxLevel = Node->Levels.Num();
		bModified = true;
	}
	
	if (FSkillTreeLevelRequirement::CanStateUpgradeLevel(State, Node->Levels, GlobalRequirement, NewNodeState.Level))
		EnsureStatus(NewNodeState.Level > 0 ? ESkillTreeElementStatus::Upgradeable : ESkillTreeElementStatus::Available);
	else
		EnsureStatus(NewNodeState.Level > 0 ? ESkillTreeElementStatus::Picked : ESkillTreeElementStatus::Locked);
	
	if (bModified)
		State->SetNodeState(NodeId, NewNodeState);
}

void USkillTreeDataAsset::GatherInterestsForNode_Implementation(const FGameplayTag& NodeId, FSkillTreeBehaviorInterest& Interests)
{
	const auto* Node = Nodes.Find(NodeId);
	if (!Node) return;
	
	for (const auto& LevelData : Node->Levels)
	{
		FSkillTreeRequirementBase::InstGatherInterests(LevelData.Requirement, Interests);
		if (LevelData.bUseGlobalRequirements)
			FSkillTreeRequirementBase::InstGatherInterests(GlobalRequirement, Interests);
	}
}
