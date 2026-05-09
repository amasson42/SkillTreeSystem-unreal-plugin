// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"
#include "SkillTreeBehaviorInterestStructures.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeBehaviorSkillInterest
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TreeName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SkillName;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeBehaviorResourceInterest
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTreeResourceType ResourceType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ResourceName;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeBehaviorInterest
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillTreeBehaviorSkillInterest> SkillInterest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillTreeBehaviorResourceInterest> ResourceInterests;
	
	void Append(const FSkillTreeBehaviorInterest& Other)
	{
		ResourceInterests.Append(Other.ResourceInterests);
		SkillInterest.Append(Other.SkillInterest);
	}
};
