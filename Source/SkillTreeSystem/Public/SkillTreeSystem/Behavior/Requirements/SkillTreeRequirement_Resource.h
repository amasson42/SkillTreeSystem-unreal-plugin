// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementBase.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "SkillTreeRequirement_Resource.generated.h"

class USkillTreeStateControllerBase;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_Resource : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ResourceName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	ESkillTreeResourceType ResourceType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource", meta = (EditCondition = "ResourceType == ESkillTreeResourceType::Boolean", EditConditionHides))
	bool ExpectTrue = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource", meta = (EditCondition = "ResourceType == ESkillTreeResourceType::Scalar", EditConditionHides))
	float MinQuantity = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource", meta = (EditCondition = "ResourceType == ESkillTreeResourceType::Integer", EditConditionHides))
	int32 MinValue = 1;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	virtual void GatherInterests(FSkillTreeBehaviorInterest& Interests) const override;
	
};
