// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_SkillLevel.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"


bool FSkillTreeRequirement_SkillLevel::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	FSkillTreeNodeState NodeState;
	State->GetNodeState(TreeCategory, NodeId, NodeState);
	return NodeState.Level >= MinLevel;
}

void FSkillTreeRequirement_SkillLevel::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	auto& SkillInterest = Interests.SkillInterest.AddDefaulted_GetRef();
	SkillInterest.TreeName = TreeCategory;
	SkillInterest.SkillName = NodeId;
}
