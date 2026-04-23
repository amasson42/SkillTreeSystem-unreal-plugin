// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Components/SkillTreeStateControllerBase.h"

const FSkillTreeResourceContainer& USkillTreeStateControllerBase::GetResourceContainer() const
{
	static FSkillTreeResourceContainer None;
	return None;
}

void USkillTreeStateControllerBase::RequestIncreaseSkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId) const
{
	OnRequestIncreaseSkill.Broadcast(TreeCategory, NodeId);
}

void USkillTreeStateControllerBase::RequestDecreaseSkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId) const
{
	OnRequestDecreaseSkill.Broadcast(TreeCategory, NodeId);
}
