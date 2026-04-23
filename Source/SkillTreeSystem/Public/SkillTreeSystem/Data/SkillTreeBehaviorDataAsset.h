// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/Structures/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/Structures/SkillTreeRequirementsStructures.h"
#include "SkillTreeBehaviorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeBehaviorDataAssetElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillTreeRequirements> LevelUpgradeRequirements;
	
};

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeBehaviorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeSystem")
	bool CanUpgradeNode(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SkillTreeSystem")
	void UpdateNodeState(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FGameplayTag TreeCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<FGameplayTag, FSkillTreeBehaviorDataAssetElement> Nodes;
	
private:
	
	bool _CanUpgradeNode(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State,
		const FSkillTreeBehaviorDataAssetElement* CachedBehavior = nullptr,
		const FSkillTreeNodeState* CachedState = nullptr) const;
	
};
