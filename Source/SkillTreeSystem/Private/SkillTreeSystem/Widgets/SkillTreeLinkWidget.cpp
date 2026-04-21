// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Widgets/SkillTreeLinkWidget.h"

void USkillTreeLinkWidget::SetAppearance(const FSkillTreeLinkAppearance& InAppearance)
{
	Appearance = InAppearance;

	UpdateAppearance(Appearance);
}

const FSkillTreeLinkAppearance& USkillTreeLinkWidget::GetAppearance() const
{
	return Appearance;
}

void USkillTreeLinkWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UpdateAppearance(Appearance);
}

#if WITH_EDITOR
void USkillTreeLinkWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	UpdateAppearance(Appearance);
}
#endif
