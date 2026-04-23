// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Structures/SkillTreeRequirementsStructures.h"

#include "SkillTreeSystem/Components/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"


bool FSkillTreeRequirementBase::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	return true;
}

bool FSkillTreeRequirement_SkillLevel::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	FSkillTreeNodeState NodeState;
	State->GetNodeState(TreeCategory, NodeId, NodeState);
	return NodeState.Level >= MinLevel;
}

bool FSkillTreeRequirement_BoolResource::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	const FSkillTreeResourceContainer& ResourceContainer = State->GetResourceContainer();
	const bool Value = ResourceContainer.GetBoolResource(ResourceName);
	return Value == ExpectTrue;
}

bool FSkillTreeRequirement_ScalarResource::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	const FSkillTreeResourceContainer& ResourceContainer = State->GetResourceContainer();
	const float Value = ResourceContainer.GetScalarResource(ResourceName);
	return Value >= MinQuantity;
}

bool FSkillTreeRequirement_IntegerResource::IsFulfilled(USkillTreeStateControllerBase* State) const
{
	if (!IsValid(State)) return false;
	
	const FSkillTreeResourceContainer& ResourceContainer = State->GetResourceContainer();
	const float Value = ResourceContainer.GetScalarResource(ResourceName);
	return Value >= MinValue;
}

bool FSkillTreeRequirements::GetFulfilled(USkillTreeStateControllerBase* State, TArray<int32>* UnfulfilledIndices) const
{
	bool Fulfilled = true;
	
	for (int32 i = 0; i < Requirements.Num(); i++)
	{
		const FInstancedStruct& ReqData = Requirements[i];
		const FSkillTreeRequirementBase* Req = ReqData.GetPtr<FSkillTreeRequirementBase>();
		if (!Req) continue;
		
		if (!Req->IsFulfilled(State))
		{
			Fulfilled = false;
			if (UnfulfilledIndices)
				UnfulfilledIndices->Add(i);
		}
	}
	
	return Fulfilled;
}
