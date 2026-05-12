// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/SkillTreeBehaviorDataAsset.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/StateController/SkillTreeStateControllerEditable.h"

void USkillTreeBehaviorDataAsset::GetNodesIds_Implementation(TArray<FGameplayTag>& OutNodes) const
{
	Nodes.GetKeys(OutNodes);
}

bool USkillTreeBehaviorDataAsset::CanUpgradeNode_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const
{
	if (const auto* NodeBehavior = Nodes.Find(NodeId))
	{
		FSkillTreeNodeState NodeState;
		State->GetNodeState(NodeId, NodeState);
		return FSkillTreeLevelRequirement::CanStateUpgradeLevel(State, NodeBehavior->Levels, GlobalRequirement, NodeState.Level);
	}
	return false;
}

void USkillTreeBehaviorDataAsset::UpdateNodeState_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State)
{
	const auto* NodeBehavior = Nodes.Find(NodeId);
	if (!NodeBehavior) return;
	
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
	
	if (NewNodeState.MaxLevel != NodeBehavior->Levels.Num())
	{
		NewNodeState.MaxLevel = NodeBehavior->Levels.Num();
		bModified = true;
	}
	
	if (FSkillTreeLevelRequirement::CanStateUpgradeLevel(State, NodeBehavior->Levels, GlobalRequirement, NewNodeState.Level))
		EnsureStatus(NewNodeState.Level > 0 ? ESkillTreeElementStatus::Upgradeable : ESkillTreeElementStatus::Available);
	else
		EnsureStatus(NewNodeState.Level > 0 ? ESkillTreeElementStatus::Picked : ESkillTreeElementStatus::Locked);
	
	if (bModified)
		State->SetNodeState(NodeId, NewNodeState);
}

void USkillTreeBehaviorDataAsset::GatherInterestsForNode_Implementation(const FGameplayTag& NodeId, FSkillTreeBehaviorInterest& Interests)
{
	const auto* NodeBehavior = Nodes.Find(NodeId);
	if (!NodeBehavior) return;
	
	for (const auto& LevelData : NodeBehavior->Levels)
	{
		FSkillTreeRequirementBase::InstGatherInterests(LevelData.Requirement, Interests);
		if (LevelData.bUseGlobalRequirements)
			FSkillTreeRequirementBase::InstGatherInterests(GlobalRequirement, Interests);
	}
}
