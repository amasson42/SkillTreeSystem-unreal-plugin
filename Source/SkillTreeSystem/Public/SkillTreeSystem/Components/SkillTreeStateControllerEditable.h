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

	TMap<FName, FSkillTreeNodeState> NodeStates;
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
	void InitTreeWithSource(const FName& TreeCategory, TScriptInterface<ISkillTreeSourceInterface> Source);
	
	virtual void GetNodeState(const FName& TreeCategory, const FName& NodeId, FSkillTreeNodeState& OutState) override;
	
	virtual void GetLinkState(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState) override;
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState")
	void SetNodeState(const FName& TreeCategory, const FName& NodeId, const FSkillTreeNodeState& InState);
	
protected:

	TMap<FName, FSkillTreeCategoryState> TreeStates;
	
};
