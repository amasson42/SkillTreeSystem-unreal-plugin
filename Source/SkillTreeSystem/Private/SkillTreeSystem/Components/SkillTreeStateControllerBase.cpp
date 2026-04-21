// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Components/SkillTreeStateControllerBase.h"

void USkillTreeStateControllerBase::RequestModifySkill(const FName& TreeCategory, const FName& NodeId, int32 NewLevel)
{
	OnRequestPickSkill.Broadcast(TreeCategory, NodeId, NewLevel);
}
