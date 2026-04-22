// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SkillTreeCoreStructures.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeLinkName
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StartNodeName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EndNodeName;
	
	bool operator==(const FSkillTreeLinkName& Other) const
	{
		return StartNodeName == Other.StartNodeName
			&& EndNodeName == Other.EndNodeName;
	}
};

FORCEINLINE uint32 GetTypeHash(const FSkillTreeLinkName& Link)
{
	return HashCombine(
		GetTypeHash(Link.StartNodeName),
		GetTypeHash(Link.EndNodeName)
	);
}

UENUM(BlueprintType)
enum class ESkillTreeElementStatus : uint8
{
	Locked = 0		UMETA(DisplayName = "Locked"),
	Available = 1	UMETA(DisplayName = "Available"),
	Picked = 2		UMETA(DisplayName = "Picked"),
	Upgradeable = 3	UMETA(DisplayName = "Upgradeable"),
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeNodeState
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTreeElementStatus Status = ESkillTreeElementStatus::Locked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel = 1;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeLinkState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTreeElementStatus Status = ESkillTreeElementStatus::Locked;

	void InitFromNodeStates(const FSkillTreeNodeState& Start, const FSkillTreeNodeState& End)
	{
		Status = FMath::Min(Start.Status, End.Status);
	}
};
