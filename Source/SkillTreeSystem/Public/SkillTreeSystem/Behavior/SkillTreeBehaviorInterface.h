// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "UObject/Interface.h"
#include "SkillTreeBehaviorInterface.generated.h"


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
	bool CanUpgradeNode(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeBehavior")
	void UpdateNodeState(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeBehavior")
	void GatherInterestsForNode(const FGameplayTag& NodeId, FSkillTreeBehaviorInterest& Interests);
	
};
