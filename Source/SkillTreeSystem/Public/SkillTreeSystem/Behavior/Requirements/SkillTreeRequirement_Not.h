// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementBase.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "SkillTreeRequirement_Not.generated.h"

class USkillTreeStateControllerBase;

USTRUCT(BlueprintType, DisplayName = "SKTS Not")
struct SKILLTREESYSTEM_API FSkillTreeRequirement_Not : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/Script/SkillTreeSystem.SkillTreeRequirementBase", ExcludeBaseStruct))
	FInstancedStruct PreventedBy;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	virtual void GatherInterests(FSkillTreeBehaviorInterest& Interests) const override;
	
};
