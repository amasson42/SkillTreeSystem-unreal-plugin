// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"

const FSkillTreeResourceContainer& USkillTreeStateControllerBase::GetResourceContainer() const
{
	static FSkillTreeResourceContainer None;
	return None;
}

void USkillTreeStateControllerBase::RequestIncreaseSkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId)
{
	OnRequestIncreaseSkill.Broadcast(this, TreeCategory, NodeId);
}

void USkillTreeStateControllerBase::RequestDecreaseSkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId)
{
	OnRequestDecreaseSkill.Broadcast(this, TreeCategory, NodeId);
}
