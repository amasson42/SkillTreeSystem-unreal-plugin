// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_AnyOf.h"


bool FSkillTreeRequirement_AnyOf::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	for (const FInstancedStruct& ReqStruct : Requirements)
	{
		if (InstIsFulfilled(ReqStruct, State))
			return true;
	}
	
	return false;
}

void FSkillTreeRequirement_AnyOf::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	for (const FInstancedStruct& ReqStruct : Requirements)
	{
		InstGatherInterests(ReqStruct, Interests);
	}
}
