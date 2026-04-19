// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/Interfaces/SkillTreeSourceInterface.h"
#include "SkillTreeSourceDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FSkillTreeSourceDataAssetElement
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillTreeNodeAppearance Appearance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillTreeNodePosition Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FSkillTreeLinkAppearance> Links;

};

/**
 * Implementation of a mono tree and mono widget type and data driven node inputs
 */
UCLASS()
class SKILLTREESYSTEM_API USkillTreeSourceDataAsset : public UDataAsset, public ISkillTreeSourceInterface
{
	GENERATED_BODY()
	
public:

	void GetNodesIds_Implementation(const FName& TreeCategory, TArray<FName>& OutNodes) override;
	void GetNodeClass_Implementation(const FName& TreeCategory, const FName& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass) override;
	void GetNodeAppearance_Implementation(const FName& TreeCategory, const FName& NodeId, FSkillTreeNodeAppearance& OutAppearance) override;
	void GetNodesPosition_Implementation(const FName& TreeCategory, TMap<FName, FSkillTreeNodePosition>& NodePositions) override;
	
	void GetLinks_Implementation(const FName& TreeCategory, TArray<FSkillTreeLinkName>& OutLinks) override;
	void GetLinkClass_Implementation(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass) override;
	void GetLinkAppearance_Implementation(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkAppearance& OutAppearance) override;
	void GetLinksPositions_Implementation(const FName& TreeCategory, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinksPositions) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<USkillTreeNodeWidget> NodeWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<USkillTreeLinkWidget> LinkWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodes")
	TMap<FName, FSkillTreeSourceDataAssetElement> Nodes;

};
