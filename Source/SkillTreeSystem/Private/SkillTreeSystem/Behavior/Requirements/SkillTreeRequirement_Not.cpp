// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_Not.h"


bool FSkillTreeRequirement_Not::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	return !InstIsFulfilled(PreventedBy, State);
}

void FSkillTreeRequirement_Not::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	InstGatherInterests(PreventedBy, Interests);
}
