// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class ISkillTreeSourceInterface;

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTree|Source")
	void SetSource(TScriptInterface<ISkillTreeSourceInterface> InSource, FName InTreeCategory);
	
protected:
	
	virtual void NativeConstruct() override;
	
#if WITH_EDITOR
	virtual void NativePreConstruct() override;
#endif
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillTree")
	void OnSourceUpdated(bool bValidSource, const TScriptInterface<ISkillTreeSourceInterface>& NewSource, const FName& NewTreeCategory);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillTree|Source", meta = (ExposeOnSpawn = true))
	TScriptInterface<ISkillTreeSourceInterface> Source;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillTree|Source", meta = (ExposeOnSpawn = true))
	FName TreeCategory;

private:
	
	void OnSourceUpdatedCheck();
};
