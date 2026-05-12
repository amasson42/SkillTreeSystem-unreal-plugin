// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/Behavior/SkillTreeBehaviorInterface.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementBase.h"
#include "SkillTreeBehaviorDataAsset.generated.h"


USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeBehaviorDataAssetElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	TArray<FSkillTreeLevelRequirement> Levels;
	
};

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeBehaviorDataAsset : public UDataAsset, public ISkillTreeBehaviorInterface
{
	GENERATED_BODY()
	
public:
	
	virtual void GetNodesIds_Implementation(TArray<FGameplayTag>& OutNodes) const override;
	virtual bool CanUpgradeNode_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const override;
	virtual void UpdateNodeState_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State) override;
	virtual void GatherInterestsForNode_Implementation(const FGameplayTag& NodeId, FSkillTreeBehaviorInterest& Interests) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<FGameplayTag, FSkillTreeBehaviorDataAssetElement> Nodes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Requirements", meta = (BaseStruct = "/Script/SkillTreeSystem.SkillTreeRequirementBase", ExcludeBaseStruct))
	FInstancedStruct GlobalRequirement;
	
};
