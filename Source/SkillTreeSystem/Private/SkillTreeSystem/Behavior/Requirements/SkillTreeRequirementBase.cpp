// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementBase.h"


bool FSkillTreeRequirementBase::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	return true;
}

void FSkillTreeRequirementBase::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	
}

bool FSkillTreeRequirementBase::InstIsFulfilled(
	const FInstancedStruct& RequirementStruct,
	USkillTreeStateControllerBase* State)
{
	const auto* Requirement = RequirementStruct.GetPtr<FSkillTreeRequirementBase>();
	if (!Requirement)
		return true;
	
	return Requirement->IsFulfilled(State);
}

void FSkillTreeRequirementBase::InstGatherInterests(
	const FInstancedStruct& RequirementStruct,
	FSkillTreeBehaviorInterest& Interests)
{
	const auto* Requirement = RequirementStruct.GetPtr<FSkillTreeRequirementBase>();
	if (!Requirement) return;
	
	return Requirement->GatherInterests(Interests);
}
