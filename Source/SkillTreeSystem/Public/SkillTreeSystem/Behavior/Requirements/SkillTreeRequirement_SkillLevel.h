// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementBase.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "SkillTreeRequirement_SkillLevel.generated.h"

class USkillTreeStateControllerBase;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_SkillLevel : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TreeCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag NodeId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinLevel = 1;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	virtual void GatherInterests(FSkillTreeBehaviorInterest& Interests) const override;
	
};
