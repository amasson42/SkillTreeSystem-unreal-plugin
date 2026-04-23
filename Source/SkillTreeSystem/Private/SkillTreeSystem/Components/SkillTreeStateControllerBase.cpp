// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Components/SkillTreeStateControllerBase.h"

const FSkillTreeResourceContainer& USkillTreeStateControllerBase::GetResourceContainer()
{
	static FSkillTreeResourceContainer None;
	return None;
}

void USkillTreeStateControllerBase::RequestModifySkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, int32 NewLevel)
{
	OnRequestPickSkill.Broadcast(TreeCategory, NodeId, NewLevel);
}
