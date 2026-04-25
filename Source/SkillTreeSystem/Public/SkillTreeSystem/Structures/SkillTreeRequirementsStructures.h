// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "SkillTreeRequirementsStructures.generated.h"

class USkillTreeStateControllerBase;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	virtual ~FSkillTreeRequirementBase() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_SkillLevel : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TreeCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag NodeId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinLevel = 1;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_BoolResource : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ResourceName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ExpectTrue = true;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_ScalarResource : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ResourceName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinQuantity = 1.0f;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_IntegerResource : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ResourceName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinValue = 1;
	
	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirement_ObjectPredicate : public FSkillTreeRequirementBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USkillTreeRequirementPredicate> PredicateClass;

	virtual bool IsFulfilled(USkillTreeStateControllerBase* State) const override;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeRequirements
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/Script/SkillTreeSystem.SkillTreeRequirementBase", ExcludeBaseStruct))
	TArray<FInstancedStruct> Requirements;
	
	bool GetFulfilled(USkillTreeStateControllerBase* State, TArray<int32>* UnfulfilledIndices = nullptr) const;
	
};
