// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCanvasStructures.h"
#include "SkillTreeCanvasSourceInterface.generated.h"

class USkillTreeNodeWidget;
class USkillTreeLinkWidget;

// This class does not need to be modified.
UINTERFACE()
class USkillTreeCanvasSourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Data source interface to implement to fill up skill tree ui.
 */
class SKILLTREESYSTEM_API ISkillTreeCanvasSourceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Nodes")
	void GetCanvasNodesIds(TArray<FGameplayTag>& OutNodes);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Nodes")
	void GetCanvasNodeData(const FGameplayTag& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass, FSkillTreeNodeAppearance& OutAppearance);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Links")
	void GetCanvasLinkIds(TArray<FSkillTreeLinkName>& OutLinks);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Links")
	void GetCanvasLinkData(const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass, FSkillTreeLinkAppearance& OutAppearance);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Positions")
	void GetCanvasElementsPositions(TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions);
	
};
