// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "SkillTreeRequirementBase.generated.h"

class USkillTreeStateControllerBase;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	virtual ~FSkillTreeRequirementBase() = default;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const;
	virtual void GatherInterests(FSkillTreeBehaviorInterest& Interests) const;
	
	static bool InstIsFulfilled(const FInstancedStruct& RequirementStruct, USkillTreeStateControllerBase* State);
	static void InstGatherInterests(const FInstancedStruct& RequirementStruct, FSkillTreeBehaviorInterest& Interests);
	
};
