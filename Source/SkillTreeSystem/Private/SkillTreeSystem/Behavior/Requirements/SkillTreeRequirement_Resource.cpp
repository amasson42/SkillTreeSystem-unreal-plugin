// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirement_Resource.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"


bool FSkillTreeRequirement_Resource::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	const FSkillTreeResourceContainer& ResourceContainer = State->GetResourceContainer();
	
	switch (ResourceType)
	{
	case ESkillTreeResourceType::Boolean:
		return ResourceContainer.GetBoolResource(ResourceName) == ExpectTrue;
	case ESkillTreeResourceType::Scalar:
		return ResourceContainer.GetScalarResource(ResourceName) >= MinQuantity;
	case ESkillTreeResourceType::Integer:
		return ResourceContainer.GetIntegerResource(ResourceName) >= MinValue;
	default:
		return true;
	}
}

void FSkillTreeRequirement_Resource::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	auto& ResourceInterest = Interests.ResourceInterests.AddDefaulted_GetRef();
	ResourceInterest.ResourceType = ResourceType;
	ResourceInterest.ResourceName = ResourceName;
}
