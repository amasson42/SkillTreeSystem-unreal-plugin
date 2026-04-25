// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainerLibrary.h"

bool USkillTreeResourceContainerLibrary::GetBoolResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName)
{
	return Container.GetBoolResource(ResourceName);
}

void USkillTreeResourceContainerLibrary::SetBoolResource(FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, bool Value)
{
	Container.SetBoolResource(ResourceName, Value);
}

float USkillTreeResourceContainerLibrary::GetScalarResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName)
{
	return Container.GetScalarResource(ResourceName);
}

void USkillTreeResourceContainerLibrary::SetScalarResource(FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, float Value)
{
	Container.SetScalarResource(ResourceName, Value);
}

int32 USkillTreeResourceContainerLibrary::GetIntegerResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName)
{
	return Container.GetIntegerResource(ResourceName);
}

void USkillTreeResourceContainerLibrary::SetIntegerResource(FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, int32 Value)
{
	Container.SetIntegerResource(ResourceName, Value);
}

const FInstancedStruct& USkillTreeResourceContainerLibrary::GetStructResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName)
{
	return Container.GetStructResource(ResourceName);
}

void USkillTreeResourceContainerLibrary::SetStructResource(FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, const FInstancedStruct& Value)
{
	Container.SetStructResource(ResourceName, Value);
}

bool USkillTreeResourceContainerLibrary::HasResource(const FSkillTreeResourceContainer& Container, ESkillTreeResourceType ResourceType, FGameplayTag ResourceName)
{
	return Container.HasResource(ResourceType, ResourceName);
}

void USkillTreeResourceContainerLibrary::RemoveResource(FSkillTreeResourceContainer& Container, ESkillTreeResourceType ResourceType, FGameplayTag ResourceName)
{
	Container.RemoveResource(ResourceType, ResourceName);
}
