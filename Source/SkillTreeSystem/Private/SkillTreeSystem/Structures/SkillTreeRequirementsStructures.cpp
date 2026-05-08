// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Structures/SkillTreeRequirementsStructures.h"

#include "SkillTreeSystem/Components/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/Requirements/SkillTreeRequirementPredicate.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"


bool FSkillTreeRequirementBase::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	return true;
}

void FSkillTreeRequirementBase::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	
}

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

bool FSkillTreeRequirements::GetFulfilled(USkillTreeStateControllerBase* State, TArray<int32>* UnfulfilledIndices) const
{
	bool bFulfilled = true;
	
	for (int32 i = 0; i < Requirements.Num(); i++)
	{
		const FInstancedStruct& ReqData = Requirements[i];
		const FSkillTreeRequirementBase* Req = ReqData.GetPtr<FSkillTreeRequirementBase>();
		if (!Req) continue;
		
		if (!Req->IsFulfilled(State))
		{
			if (UnfulfilledIndices)
			{
				UnfulfilledIndices->Add(i);
				bFulfilled = false;
			}
			else
				return false;
		}
	}
	
	return bFulfilled;
}

void FSkillTreeRequirements::GatherInterests(FSkillTreeBehaviorInterest& Interests) const
{
	for (const FInstancedStruct& ReqData : Requirements)
	{
		const FSkillTreeRequirementBase* Req = ReqData.GetPtr<FSkillTreeRequirementBase>();
		if (!Req) continue;
		
		Req->GatherInterests(Interests);
	}
}
