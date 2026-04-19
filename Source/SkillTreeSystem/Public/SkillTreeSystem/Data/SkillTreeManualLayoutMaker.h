// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/Interfaces/SkillTreeLayoutMaker.h"
#include "SkillTreeManualLayoutMaker.generated.h"

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeManualLayoutElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSkillTreeNodePosition Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> Links;

};

/**
 * 
 */
UCLASS()
class SKILLTREESYSTEM_API USkillTreeManualLayoutMaker : public UDataAsset, public ISkillTreeLayoutMaker
{
	GENERATED_BODY()
	
public:
	
	virtual void GetElementsPosition_Implementation(TMap<FName, FSkillTreeNodePosition>& NodePositions, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& LinkPositions) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodes")
	TMap<FName, FSkillTreeManualLayoutElement> Nodes;
	
};
