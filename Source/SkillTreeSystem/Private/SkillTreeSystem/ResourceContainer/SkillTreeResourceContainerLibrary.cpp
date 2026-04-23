// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainerLibrary.h"

bool USkillTreeResourceContainerLibrary::GetBoolResource(const FSkillTreeResourceContainer& Container, FGameplayTag Tag)
{
	return Container.GetBoolResource(Tag);
}

void USkillTreeResourceContainerLibrary::SetBoolResource(FSkillTreeResourceContainer& Container, FGameplayTag Tag, bool Value)
{
	Container.SetBoolResource(Tag, Value);
}

float USkillTreeResourceContainerLibrary::GetScalarResource(const FSkillTreeResourceContainer& Container, FGameplayTag Tag)
{
	return Container.GetScalarResource(Tag);
}

void USkillTreeResourceContainerLibrary::SetScalarResource(FSkillTreeResourceContainer& Container, FGameplayTag Tag, float Value)
{
	Container.SetScalarResource(Tag, Value);
}

int32 USkillTreeResourceContainerLibrary::GetIntegerResource(const FSkillTreeResourceContainer& Container, FGameplayTag Tag)
{
	return Container.GetIntegerResource(Tag);
}

void USkillTreeResourceContainerLibrary::SetIntegerResource(FSkillTreeResourceContainer& Container, FGameplayTag Tag, int32 Value)
{
	Container.SetIntegerResource(Tag, Value);
}
