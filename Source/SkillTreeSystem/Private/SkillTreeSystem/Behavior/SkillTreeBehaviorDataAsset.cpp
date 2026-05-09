// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/SkillTreeBehaviorDataAsset.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/StateController/SkillTreeStateControllerEditable.h"

bool USkillTreeBehaviorDataAsset::CanUpgradeNode_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const
{
	return _CanUpgradeNode(NodeId, State);
}

void USkillTreeBehaviorDataAsset::UpdateNodeState_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State)
{
	const auto* NodeBehavior = Nodes.Find(NodeId);
	if (!NodeBehavior) return;
	
	FSkillTreeNodeState NewNodeState;
	State->GetNodeState(TreeCategory, NodeId, NewNodeState);
	
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
	
	if (_CanUpgradeNode(NodeId, State, NodeBehavior, &NewNodeState))
	{
		EnsureStatus(NewNodeState.Level > 0 ? ESkillTreeElementStatus::Upgradeable : ESkillTreeElementStatus::Available);
	}
	else
	{
		EnsureStatus(NewNodeState.Level > 0 ? ESkillTreeElementStatus::Picked : ESkillTreeElementStatus::Locked);
	}
	
	if (bModified)
		State->SetNodeState(TreeCategory, NodeId, NewNodeState);
}

void USkillTreeBehaviorDataAsset::GatherInterestsForNode_Implementation(const FGameplayTag& NodeId, FSkillTreeBehaviorInterest& Interests)
{
	const auto* NodeBehavior = Nodes.Find(NodeId);
	if (!NodeBehavior) return;
	
	for (const auto& LevelData : NodeBehavior->Levels)
	{
		FSkillTreeRequirementBase::InstGatherInterests(LevelData.Requirement, Interests);
		if (!LevelData.bIgnoreGlobalRequirements)
			FSkillTreeRequirementBase::InstGatherInterests(GlobalRequirement, Interests);
	}
}

bool USkillTreeBehaviorDataAsset::_CanUpgradeNode(
	const FGameplayTag& NodeId,
	USkillTreeStateControllerBase* State,
	const FSkillTreeBehaviorDataAssetElement* CachedBehavior,
	const FSkillTreeNodeState* CachedState) const
{
	FSkillTreeNodeState NodeState;
	if (CachedState)
		NodeState = *CachedState;
	else
		State->GetNodeState(TreeCategory, NodeId, NodeState);
	
	const auto* NodeBehavior = CachedBehavior ? CachedBehavior : Nodes.Find(NodeId);
	if (!NodeBehavior) return false;
	
	if (NodeState.Level >= NodeBehavior->Levels.Num())
		return false;
	
	if (NodeState.Level < 0)
		return false;
	
	const auto& LevelData = NodeBehavior->Levels[NodeState.Level];
	
	if (!LevelData.bIgnoreGlobalRequirements)
	{
		if (!FSkillTreeRequirementBase::InstIsFulfilled(GlobalRequirement, State))
			return false;
	}
	
	if (!FSkillTreeRequirementBase::InstIsFulfilled(LevelData.Requirement, State))
		return false;
	
	return true;
}
