// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeSystem/Structures/SkillTreeStructures.h"
#include "SkillTreeLinkWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeLinkWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeLink")
	void SetAppearance(const FSkillTreeLinkAppearance& InAppearance);
	
	UFUNCTION(BlueprintPure, Category = "SkillTreeLink")
	const FSkillTreeLinkAppearance& GetAppearance() const;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillTreeLink")
	void SetCanvasPosition(const FSkillTreeLinkPosition& Position, UCanvasPanelSlot* CanvasSlot);
	
protected:
	
	virtual void NativeConstruct() override;
	
#if WITH_EDITOR
	virtual void NativePreConstruct() override;
#endif
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillTreeLink")
	void UpdateAppearance(const FSkillTreeLinkAppearance& NewAppearance);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillTreeLink", meta = (ExposeOnSpawn = true))
	FSkillTreeLinkAppearance Appearance;

};
