// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCanvasStructures.h"
#include "SkillTreeSystem/CanvasSource/LayoutMaker/SkillTreeLayoutMaker.h"
#include "SkillTreeManualLayoutMaker.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeManualLayoutElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSkillTreeNodePosition Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FGameplayTag> Links;

};

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeManualLayoutMaker : public UDataAsset, public ISkillTreeLayoutMaker
{
	GENERATED_BODY()
	
public:
	
	virtual void GetCanvasElementsPositions_Implementation(
		TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
		TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<FGameplayTag, FSkillTreeManualLayoutElement> Nodes;
	
};
