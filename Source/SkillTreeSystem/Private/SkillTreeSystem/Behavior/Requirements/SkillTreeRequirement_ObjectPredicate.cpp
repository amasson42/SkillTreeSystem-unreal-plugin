// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_ObjectPredicate.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementPredicate.h"


bool FSkillTreeRequirement_ObjectPredicate::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State) || !PredicateClass) return false;
	
	const auto* Predicate = PredicateClass->GetDefaultObject<USkillTreeRequirementPredicate>();
	if (!Predicate) return false;
	
	return Predicate->IsFulfilled(State);
}

void FSkillTreeRequirement_ObjectPredicate::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	if (!PredicateClass) return;
	
	const auto* Predicate = PredicateClass->GetDefaultObject<USkillTreeRequirementPredicate>();
	if (!Predicate) return;
	
	FSkillTreeBehaviorInterest NewInterests;
	Predicate->GatherInterests(NewInterests);
	
	Interests.Append(NewInterests);
}
