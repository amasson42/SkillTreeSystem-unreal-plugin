// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Widgets/SkillTreeNodeWidget.h"

void USkillTreeNodeWidget::SetAppearance(const FSkillTreeNodeAppearance& InAppearance)
{
	Appearance = InAppearance;

	UpdateAppearance(Appearance);
}

const FSkillTreeNodeAppearance& USkillTreeNodeWidget::GetAppearance() const
{
	return Appearance;
}

void USkillTreeNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UpdateAppearance(Appearance);
}

#if WITH_EDITOR
void USkillTreeNodeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	UpdateAppearance(Appearance);
}
#endif

void USkillTreeNodeWidget::CallNodeClicked(bool bRightClick, bool bDoubleClick)
{
	if (bRightClick)
	{
		OnNodeRightClicked.Broadcast(this, NodeId);
	}
	else
	{
		if (bDoubleClick)
		{
			OnNodeDoubleClicked.Broadcast(this, NodeId);
		}
		else
		{
			OnNodeClicked.Broadcast(this, NodeId);
		}
	}
}
