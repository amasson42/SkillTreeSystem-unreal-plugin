// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/Interfaces/SkillTreeSourceInterface.h"
#include "SkillTreeSourceDataAsset.generated.h"

class ISkillTreeLayoutMaker;

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeSourceDataAssetElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillTreeNodeAppearance Appearance;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeSourceDataAssetTreeCategory
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
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


/**
 * Implementation of a mono tree and mono widget type and data driven node inputs
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeSourceDataAsset : public UDataAsset, public ISkillTreeSourceInterface
{
	GENERATED_BODY()
	
public:
	
	void GetNodesIds_Implementation(const FGameplayTag& TreeCategory, TArray<FGameplayTag>& OutNodes) override;
	void GetNodeClass_Implementation(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass) override;
	void GetNodeAppearance_Implementation(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, FSkillTreeNodeAppearance& OutAppearance) override;
	void GetNodesPosition_Implementation(const FGameplayTag& TreeCategory, TMap<FGameplayTag, FSkillTreeNodePosition>& NodePositions) override;
	
	void GetLinks_Implementation(const FGameplayTag& TreeCategory, TArray<FSkillTreeLinkName>& OutLinks) override;
	void GetLinkClass_Implementation(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass) override;
	void GetLinkAppearance_Implementation(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkAppearance& OutAppearance) override;
	void GetLinksPositions_Implementation(const FGameplayTag& TreeCategory, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinksPositions) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<FGameplayTag, FSkillTreeSourceDataAssetTreeCategory> Trees;
	
};
