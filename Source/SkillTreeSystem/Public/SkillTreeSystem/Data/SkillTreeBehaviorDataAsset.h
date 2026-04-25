// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/Interfaces/SkillTreeBehaviorInterface.h"
#include "SkillTreeSystem/Structures/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/Structures/SkillTreeRequirementsStructures.h"
#include "SkillTreeBehaviorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeBehaviorLevelData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
	FSkillTreeRequirements Requirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
	bool bIgnoreGlobalRequirements = false;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeBehaviorDataAssetElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	TArray<FSkillTreeBehaviorLevelData> Levels;
	
};

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeBehaviorDataAsset : public UDataAsset, public ISkillTreeBehaviorInterface
{
	GENERATED_BODY()
	
public:
	
	virtual bool CanUpgradeNode_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) override;
	virtual void UpdateNodeState_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FGameplayTag TreeCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<FGameplayTag, FSkillTreeBehaviorDataAssetElement> Nodes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Requirements")
	FSkillTreeRequirements GlobalRequirements;
	
private:
	
	bool _CanUpgradeNode(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State,
		const FSkillTreeBehaviorDataAssetElement* CachedBehavior = nullptr,
		const FSkillTreeNodeState* CachedState = nullptr) const;
	
};
