// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkillTreeLayoutMaker.h"
#include "Engine/DataAsset.h"
#include "SkillTreeHierarchicalLayoutMaker.generated.h"

UENUM(BlueprintType)
enum class ESkillTreeHierarchicalLayoutDirection : uint8
{
	TopToBottom,
	BottomToTop,
	LeftToRight,
	RightToLeft
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeHierarchicalLayoutElement
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
UCLASS()
class SKILLTREESYSTEM_API USkillTreeHierarchicalLayoutMaker : public UDataAsset, public ISkillTreeLayoutMaker
{
	GENERATED_BODY()
	
public:
	
	virtual void GetCanvasElementsPositions_Implementation(
		TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
		TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodes")
	TArray<FSkillTreeHierarchicalLayoutElement> Nodes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float LevelSpacing = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float SiblingSpacing = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	ESkillTreeHierarchicalLayoutDirection Direction = ESkillTreeHierarchicalLayoutDirection::TopToBottom;
	
};
