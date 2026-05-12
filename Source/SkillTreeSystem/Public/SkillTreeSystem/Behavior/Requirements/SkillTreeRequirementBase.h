// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "SkillTreeRequirementBase.generated.h"

class USkillTreeStateControllerBase;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	virtual ~FSkillTreeRequirementBase() = default;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const;
	virtual void GatherInterests(FSkillTreeBehaviorInterest& Interests) const;
	
	static bool InstIsFulfilled(const FInstancedStruct& RequirementStruct, USkillTreeStateControllerBase* State);
	static void InstGatherInterests(const FInstancedStruct& RequirementStruct, FSkillTreeBehaviorInterest& Interests);
	
};


USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeLevelRequirement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/Script/SkillTreeSystem.SkillTreeRequirementBase", ExcludeBaseStruct))
	FInstancedStruct Requirement;
	
	/** Also check if the previous level requirements are met. Ignored if level is zero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External")
	bool bUsePreviousLevelRequirements = true;
	
	/** Also check if the global requirements are met. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External")
	bool bUseGlobalRequirements = true;
	
	static bool CanStateUpgradeLevel(
		USkillTreeStateControllerBase* State,
		const TArray<FSkillTreeLevelRequirement>& LevelRequirements,
		const FInstancedStruct& GlobalRequirement,
		int32 AimedLevel,
		bool bIgnoreGlobal = false);
	
};
