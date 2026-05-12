// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/CanvasSource/LayoutMaker/SkillTreeLayoutMaker.h"
#include "SkillTreeCircularLayoutMaker.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeCircularLayoutElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Parent;
	
};

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeCircularLayoutMaker : public UDataAsset, public ISkillTreeLayoutMaker
{
	GENERATED_BODY()
	
public:
	
	virtual void GetCanvasElementsPositions_Implementation(
		TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
		TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodes")
	TArray<FSkillTreeCircularLayoutElement> Nodes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float RootCircleRadius = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float CirclesDistance = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float ArcStartAngle = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float ArcEndAngle = 90.0f + 360.0f;
	
};
