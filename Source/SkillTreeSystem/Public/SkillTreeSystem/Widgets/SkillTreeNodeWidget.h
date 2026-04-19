// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeSystem/Structures/SkillTreeStructures.h"
#include "SkillTreeNodeWidget.generated.h"

class UCanvasPanelSlot;
/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeNodeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeNode")
	void SetAppearance(const FSkillTreeNodeAppearance& InAppearance);
	
	UFUNCTION(BlueprintPure, Category = "SkillTreeNode")
	const FSkillTreeNodeAppearance& GetAppearance() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillTreeNode")
	void SetCanvasPosition(const FSkillTreeNodePosition& Position, UCanvasPanelSlot* CanvasSlot);

protected:
	
	virtual void NativeConstruct() override;
	
#if WITH_EDITOR
	virtual void NativePreConstruct() override;
#endif
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillTreeNode")
	void UpdateAppearance(const FSkillTreeNodeAppearance& NewAppearance);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillTreeNode", meta = (ExposeOnSpawn = true))
	FSkillTreeNodeAppearance Appearance;

};
