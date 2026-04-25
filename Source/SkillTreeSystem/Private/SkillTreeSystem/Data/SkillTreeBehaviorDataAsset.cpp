// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Data/SkillTreeBehaviorDataAsset.h"

#include "SkillTreeSystem/Components/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/Components/SkillTreeStateControllerEditable.h"

bool USkillTreeBehaviorDataAsset::CanUpgradeNode_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State)
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
		TArray<int32> ErroredReasons;
		if (!GlobalRequirements.GetFulfilled(State, &ErroredReasons))
			return false;
	}
	
	TArray<int32> ErroredReasons;
	if (!LevelData.Requirements.GetFulfilled(State, &ErroredReasons))
		return false;
	
	return true;
}
