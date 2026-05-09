// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_Resource.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"


bool FSkillTreeRequirement_BoolResource::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	const FSkillTreeResourceContainer& ResourceContainer = State->GetResourceContainer();
	const bool Value = ResourceContainer.GetBoolResource(ResourceName);
	return Value == ExpectTrue;
}

void FSkillTreeRequirement_BoolResource::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	auto& ResourceInterest = Interests.ResourceInterests.AddDefaulted_GetRef();
	ResourceInterest.ResourceType = ESkillTreeResourceType::Boolean;
	ResourceInterest.ResourceName = ResourceName;
}

bool FSkillTreeRequirement_ScalarResource::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	const FSkillTreeResourceContainer& ResourceContainer = State->GetResourceContainer();
	const float Value = ResourceContainer.GetScalarResource(ResourceName);
	return Value >= MinQuantity;
}

void FSkillTreeRequirement_ScalarResource::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	auto& ResourceInterest = Interests.ResourceInterests.AddDefaulted_GetRef();
	ResourceInterest.ResourceType = ESkillTreeResourceType::Scalar;
	ResourceInterest.ResourceName = ResourceName;
}

bool FSkillTreeRequirement_IntegerResource::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	const FSkillTreeResourceContainer& ResourceContainer = State->GetResourceContainer();
	const float Value = ResourceContainer.GetIntegerResource(ResourceName);
	return Value >= MinValue;
}

void FSkillTreeRequirement_IntegerResource::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	auto& ResourceInterest = Interests.ResourceInterests.AddDefaulted_GetRef();
	ResourceInterest.ResourceType = ESkillTreeResourceType::Integer;
	ResourceInterest.ResourceName = ResourceName;
}
