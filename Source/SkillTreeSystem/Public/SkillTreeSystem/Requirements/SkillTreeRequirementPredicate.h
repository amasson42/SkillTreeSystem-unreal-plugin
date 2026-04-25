// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillTreeRequirementPredicate.generated.h"

class USkillTreeStateControllerBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Const)
class SKILLTREESYSTEM_API USkillTreeRequirementPredicate : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillTree|Requirement")
	bool IsFulfilled(USkillTreeStateControllerBase* State) const;
	
};
