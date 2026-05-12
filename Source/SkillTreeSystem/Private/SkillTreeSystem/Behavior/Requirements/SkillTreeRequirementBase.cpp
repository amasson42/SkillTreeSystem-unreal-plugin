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

bool FSkillTreeLevelRequirement::CanStateUpgradeLevel(
	USkillTreeStateControllerBase* State,
	const TArray<FSkillTreeLevelRequirement>& LevelRequirements,
	const FInstancedStruct& GlobalRequirement,
	int32 AimedLevel,
	bool bIgnoreGlobal)
{
	if (AimedLevel >= LevelRequirements.Num())
		return false;
	
	if (AimedLevel < 0)
		return false;
	
	const auto& LevelData = LevelRequirements[AimedLevel];
	
	if (LevelData.bUseGlobalRequirements && !bIgnoreGlobal)
	{
		if (!FSkillTreeRequirementBase::InstIsFulfilled(GlobalRequirement, State))
			return false;
	}
	
	if (LevelData.bUsePreviousLevelRequirements && AimedLevel > 0)
	{
		if (!CanStateUpgradeLevel(State, LevelRequirements, GlobalRequirement, AimedLevel - 1))
			return false;
	}
	
	if (!FSkillTreeRequirementBase::InstIsFulfilled(LevelData.Requirement, State))
		return false;
	
	return true;
}
