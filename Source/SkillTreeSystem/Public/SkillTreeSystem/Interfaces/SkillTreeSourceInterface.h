// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillTreeSystem/Structures/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/Structures/SkillTreeCanvasStructures.h"
#include "SkillTreeSourceInterface.generated.h"

class USkillTreeNodeWidget;
class USkillTreeLinkWidget;

// This class does not need to be modified.
UINTERFACE()
class USkillTreeSourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Data source interface to implement to fill up skill tree ui.
 * A source may be used for different data tree. The TreeCategory is the name of the skill panel opened (eg. "FireSpells", "FrostSpells", "MeleeAbilities")
 */
class SKILLTREESYSTEM_API ISkillTreeSourceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Nodes")
	void GetNodesIds(const FGameplayTag& TreeCategory, TArray<FGameplayTag>& OutNodes);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Nodes")
	void GetNodeClass(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Nodes")
	void GetNodeAppearance(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, FSkillTreeNodeAppearance& OutAppearance);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Nodes")
	void GetNodesPosition(const FGameplayTag& TreeCategory, TMap<FGameplayTag, FSkillTreeNodePosition>& NodePositions);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Links")
	void GetLinks(const FGameplayTag& TreeCategory, TArray<FSkillTreeLinkName>& OutLinks);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Links")
	void GetLinkClass(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Links")
	void GetLinkAppearance(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkAppearance& OutAppearance);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeSourceInterface|Links")
	void GetLinksPositions(const FGameplayTag& TreeCategory, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinksPositions);
	
};
