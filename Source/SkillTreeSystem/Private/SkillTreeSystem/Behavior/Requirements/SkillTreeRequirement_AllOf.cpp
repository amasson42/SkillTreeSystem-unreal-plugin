// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_AllOf.h"


bool FSkillTreeRequirement_AllOf::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	for (const FInstancedStruct& ReqStruct : Requirements)
	{
		if (!InstIsFulfilled(ReqStruct, State))
			return false;
	}
	
	return true;
}

void FSkillTreeRequirement_AllOf::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	for (const FInstancedStruct& ReqStruct : Requirements)
	{
		InstGatherInterests(ReqStruct, Interests);
	}
}
