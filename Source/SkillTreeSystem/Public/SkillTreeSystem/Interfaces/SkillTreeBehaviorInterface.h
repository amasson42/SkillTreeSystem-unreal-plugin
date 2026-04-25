// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"
#include "UObject/Interface.h"
#include "SkillTreeBehaviorInterface.generated.h"

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
	
};

// This class does not need to be modified.
UINTERFACE()
class USkillTreeBehaviorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SKILLTREESYSTEM_API ISkillTreeBehaviorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeBehavior")
	bool CanUpgradeNode(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeBehavior")
	void UpdateNodeState(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State);
	
};
