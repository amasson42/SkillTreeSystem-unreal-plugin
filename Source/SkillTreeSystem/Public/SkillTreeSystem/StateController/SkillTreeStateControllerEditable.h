// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeStateControllerBase.h"
#include "SkillTreeStateControllerEditable.generated.h"


class ISkillTreeCanvasSourceInterface;

/**
 * 
 */
UCLASS()
class SKILLTREESYSTEM_API USkillTreeStateControllerEditable : public USkillTreeStateControllerBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeSystem")
	void InitTreeWithSource(TScriptInterface<ISkillTreeCanvasSourceInterface> Source);
	
	virtual void GetNodeState(const FGameplayTag& NodeId, FSkillTreeNodeState& OutState) const override;
	
	virtual void GetLinkState(const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState) const override;
	
	virtual const FSkillTreeResourceContainer& GetResourceContainer() const override;
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState")
	void SetNodeState(const FGameplayTag& NodeId, const FSkillTreeNodeState& InState);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState")
	void SetNodeLevel(const FGameplayTag& NodeId, int32 NewLevel);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState|Resources")
	void SetBoolResource(FGameplayTag ResourceName, bool Value);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState|Resources")
	void SetScalarResource(FGameplayTag ResourceName, float Value);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState|Resources")
	void SetIntegerResource(FGameplayTag ResourceName, int32 Value);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeState|Resources")
	void SetStructResource(FGameplayTag ResourceName, const FInstancedStruct& Value);
	
protected:
	
	TMap<FGameplayTag, FSkillTreeNodeState> NodeStates;
	TMap<FSkillTreeLinkName, FSkillTreeLinkState> LinkStates;
	
	FSkillTreeResourceContainer ResourceContainer;
	
};
