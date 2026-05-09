// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_Group.h"


bool FSkillTreeRequirement_Group::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	switch (GroupType)
	{
	case ESkillTreeRequirementGroupType::AllOf:
		for (const FInstancedStruct& ReqStruct : Requirements)
		{
			if (!InstIsFulfilled(ReqStruct, State))
				return false;
		}
		return true;
	case ESkillTreeRequirementGroupType::AnyOf:
		for (const FInstancedStruct& ReqStruct : Requirements)
		{
			if (InstIsFulfilled(ReqStruct, State))
				return true;
		}
		return false;
	default:
		return true;
	}
}

void FSkillTreeRequirement_Group::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	for (const FInstancedStruct& ReqStruct : Requirements)
	{
		InstGatherInterests(ReqStruct, Interests);
	}
}
