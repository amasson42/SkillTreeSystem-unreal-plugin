// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeRequirementBase.h"
#include "SkillTreeSystem/Behavior/Requirements/SkillTreeBehaviorInterestStructures.h"
#include "SkillTreeRequirement_Group.generated.h"

class USkillTreeStateControllerBase;

UENUM(BlueprintType)
enum class ESkillTreeRequirementGroupType : uint8
{
	/** All requirements must be satisfied */
	AllOf = 0		UMETA(DisplayName = "AllOf"),
	/** Any requirement must be satisfied */
	AnyOf = 1		UMETA(DisplayName = "AnyOf"),
};

USTRUCT(BlueprintType, DisplayName = "SKTS Group")
struct SKILLTREESYSTEM_API FSkillTreeRequirement_Group : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTreeRequirementGroupType GroupType = ESkillTreeRequirementGroupType::AllOf;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/Script/SkillTreeSystem.SkillTreeRequirementBase", ExcludeBaseStruct))
	TArray<FInstancedStruct> Requirements;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	virtual void GatherInterests(FSkillTreeBehaviorInterest& Interests) const override;
	
};
