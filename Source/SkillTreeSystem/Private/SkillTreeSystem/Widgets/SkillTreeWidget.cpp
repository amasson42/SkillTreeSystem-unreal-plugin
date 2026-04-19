// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Widgets/SkillTreeWidget.h"


void USkillTreeWidget::SetSource(TScriptInterface<ISkillTreeSourceInterface> InSource, FName InTreeCategory)
{
	Source = InSource;
	TreeCategory = InTreeCategory;
	
	OnSourceUpdatedCheck();
}

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	OnSourceUpdatedCheck();
}

#if WITH_EDITOR
void USkillTreeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	OnSourceUpdatedCheck();
}
#endif


void USkillTreeWidget::OnSourceUpdatedCheck()
{
	bool bValidSource = Source && IsValid(Source.GetObject());
	
	OnSourceUpdated(bValidSource, Source, TreeCategory);
}
