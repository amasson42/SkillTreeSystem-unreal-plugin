// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillTreeCanvasStructures.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeNodeAppearance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Label;
	
	// Fill up generic node appearance parameters
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeNodePosition
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CanvasLocation;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeLinkAppearance
{
	GENERATED_BODY()
	
public:
	
	// Fill up generic link appearance parameters
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeLinkPosition
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CanvasStartLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CanvasEndLocation;
	
};
