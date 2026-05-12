// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "SkillTreeWidget.generated.h"

class USkillTreeStateControllerBase;
class ISkillTreeCanvasSourceInterface;

/**
 * 
 */
UCLASS(Abstract)
class SKILLTREESYSTEM_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTree|Source")
	virtual void SetSource(TScriptInterface<ISkillTreeCanvasSourceInterface> InSource);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTree|State")
	virtual void SetStateController(USkillTreeStateControllerBase* InStateController);
	
protected:
	
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
#if WITH_EDITOR
	virtual void NativePreConstruct() override;
#endif
	
	void ClearStateController();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillTree")
	void OnSourceUpdated(bool bValidSource, const TScriptInterface<ISkillTreeCanvasSourceInterface>& NewSource);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillTree")
	void OnNodeStateUpdated(const FGameplayTag& NodeId, const FSkillTreeNodeState& NewNodeState);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillTree")
	void OnLinkStateUpdated(const FSkillTreeLinkName& LinkName, const FSkillTreeLinkState& NewLinkState);
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillTree|Source", meta = (ExposeOnSpawn = true))
	TScriptInterface<ISkillTreeCanvasSourceInterface> Source;
	
	UPROPERTY(BlueprintReadOnly, Category = "SkillTree|State")
	TWeakObjectPtr<USkillTreeStateControllerBase> StateController;
	
private:
	
	UFUNCTION()
	void OnSkillTreeNodeUpdated(
		USkillTreeStateControllerBase* Controller,
		const FGameplayTag& NodeId,
		const FSkillTreeNodeState& State);
	
	UFUNCTION()
	void OnSkillTreeLinkUpdated(
		USkillTreeStateControllerBase* Controller,
		const FSkillTreeLinkName& LinkName,
		const FSkillTreeLinkState& State);
	
	void OnSourceUpdatedCheck();
};
