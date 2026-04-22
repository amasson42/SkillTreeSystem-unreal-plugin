// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeStateControllerBase.h"
#include "SkillTreeStateControllerEditable.generated.h"

class ISkillTreeSourceInterface;

USTRUCT()
struct SKILLTREESYSTEM_API FSkillTreeCategoryState
{
	GENERATED_BODY()

public:

	TMap<FGameplayTag, FSkillTreeNodeState> NodeStates;
	TMap<FSkillTreeLinkName, FSkillTreeLinkState> LinkStates;
	
};

/**
 * 
 */
UCLASS()
class SKILLTREESYSTEM_API USkillTreeStateControllerEditable : public USkillTreeStateControllerBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "SkillTreeSystem")
	void InitTreeWithSource(const FGameplayTag& TreeCategory, TScriptInterface<ISkillTreeSourceInterface> Source);
	
	virtual void GetNodeState(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, FSkillTreeNodeState& OutState) override;
	
	virtual void GetLinkState(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState) override;
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState")
	void SetNodeState(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, const FSkillTreeNodeState& InState);
	
protected:
	
	TMap<FGameplayTag, FSkillTreeCategoryState> TreeStates;
	
};
