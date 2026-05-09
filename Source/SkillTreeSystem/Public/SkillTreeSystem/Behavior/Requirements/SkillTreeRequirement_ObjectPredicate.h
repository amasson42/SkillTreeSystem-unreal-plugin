// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementBase.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "SkillTreeRequirement_ObjectPredicate.generated.h"

class USkillTreeStateControllerBase;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_ObjectPredicate : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USkillTreeRequirementPredicate> PredicateClass;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	virtual void GatherInterests(FSkillTreeBehaviorInterest& Interests) const override;
	
};
