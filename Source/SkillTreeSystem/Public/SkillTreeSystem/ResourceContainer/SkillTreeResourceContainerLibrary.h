// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"
#include "SkillTreeResourceContainerLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTREESYSTEM_API USkillTreeResourceContainerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	static bool GetBoolResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void SetBoolResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, bool Value);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	static float GetScalarResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void SetScalarResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, float Value);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	static int32 GetIntegerResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void SetIntegerResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, int32 Value);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	static const FInstancedStruct& GetStructResource(const FSkillTreeResourceContainer& Container, FGameplayTag ResourceName);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void SetStructResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag ResourceName, const FInstancedStruct& Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	static bool HasResource(const FSkillTreeResourceContainer& Container, ESkillTreeResourceType ResourceType, FGameplayTag ResourceName);

	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void RemoveResource(UPARAM(ref) FSkillTreeResourceContainer& Container, ESkillTreeResourceType ResourceType, FGameplayTag ResourceName);
	
};
