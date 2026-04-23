// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Data/SkillTreeBehaviorDataAsset.h"

#include "SkillTreeSystem/Components/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/Components/SkillTreeStateControllerEditable.h"

bool USkillTreeBehaviorDataAsset::CanUpgradeNode(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const
{
	return _CanUpgradeNode(NodeId, State);
}

void USkillTreeBehaviorDataAsset::UpdateNodeState(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State) const
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
	
	if (NewNodeState.MaxLevel != NodeBehavior->LevelUpgradeRequirements.Num())
	{
		NewNodeState.MaxLevel = NodeBehavior->LevelUpgradeRequirements.Num();
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
	
	if (NodeState.Level >= NodeBehavior->LevelUpgradeRequirements.Num())
		return false;
	
	if (NodeState.Level < 0)
		return false;
	
	if (!NodeBehavior->LevelUpgradeRequirements[NodeState.Level].GetFulfilled(State))
		return false;
	
	return true;
}
