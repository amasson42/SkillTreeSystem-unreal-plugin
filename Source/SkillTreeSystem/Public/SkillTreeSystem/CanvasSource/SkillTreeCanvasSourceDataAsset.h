// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCanvasSourceInterface.h"
#include "SkillTreeCanvasSourceDataAsset.generated.h"

class ISkillTreeLayoutMaker;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeSourceDataAssetElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillTreeNodeAppearance Appearance;
	
};


/**
 * Implementation of a mono tree and mono widget type and data driven node inputs
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeCanvasSourceDataAsset : public UDataAsset, public ISkillTreeCanvasSourceInterface
{
	GENERATED_BODY()
	
public:
	
	virtual void GetCanvasNodesIds_Implementation(TArray<FGameplayTag>& OutNodes) override;
	virtual void GetCanvasNodeData_Implementation(const FGameplayTag& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass, FSkillTreeNodeAppearance& OutAppearance) override;
	virtual void GetCanvasLinkIds_Implementation(TArray<FSkillTreeLinkName>& OutLinks) override;
	virtual void GetCanvasLinkData_Implementation(const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass, FSkillTreeLinkAppearance& OutAppearance) override;
	virtual void GetCanvasElementsPositions_Implementation(
		TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
		TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<FGameplayTag, FSkillTreeSourceDataAssetElement> Nodes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Node")
	TSubclassOf<USkillTreeNodeWidget> NodeWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Link")
	TSubclassOf<USkillTreeLinkWidget> LinkWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget|Link")
	FSkillTreeLinkAppearance LinkAppearance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	TScriptInterface<ISkillTreeLayoutMaker> LayoutSource;
	
};
