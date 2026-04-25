// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "SkillTreeResourceContainer.generated.h"

UENUM(BlueprintType)
enum class ESkillTreeResourceType : uint8
{
	Boolean = 0		UMETA(DisplayName = "Boolean"),
	Scalar = 1		UMETA(DisplayName = "Scalar"),
	Integer = 2		UMETA(DisplayName = "Integer"),
	Structure = 3	UMETA(DisplayName = "Structure"),
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeResourceContainer
{
	GENERATED_BODY()
	
public:
	
	bool GetBoolResource(const FGameplayTag& ResourceName) const
	{
		return BoolResources.Contains(ResourceName);
	}
	
	void SetBoolResource(const FGameplayTag& ResourceName, bool Value)
	{
		if (Value)
			BoolResources.Add(ResourceName);
		else
			BoolResources.Remove(ResourceName);
	}
	
	float GetScalarResource(const FGameplayTag& ResourceName) const
	{
		if (const float* ValuePtr = ScalarResources.Find(ResourceName))
			return *ValuePtr;
		return 0.0f;
	}

	void SetScalarResource(const FGameplayTag& ResourceName, float Value)
	{
		ScalarResources.Add(ResourceName, Value);
	}
	
	int32 GetIntegerResource(const FGameplayTag& ResourceName) const
	{
		if (const int32* ValuePtr = IntegerResources.Find(ResourceName))
			return *ValuePtr;
		return 0;
	}
	
	void SetIntegerResource(const FGameplayTag& ResourceName, int32 Value)
	{
		IntegerResources.Add(ResourceName, Value);
	}
	
	const FInstancedStruct& GetStructResource(const FGameplayTag& ResourceName) const
	{
		const static FInstancedStruct NoneInstance;
		if (const FInstancedStruct* ValuePtr = StructResources.Find(ResourceName))
			return *ValuePtr;
		return NoneInstance;
	}
	
	void SetStructResource(const FGameplayTag& ResourceName, const FInstancedStruct& Value)
	{
		StructResources.Add(ResourceName, Value);
	}
	
	FInstancedStruct* GetStructResourceMutable(const FGameplayTag& ResourceName)
	{
		return StructResources.Find(ResourceName);
	}
	
	bool HasResource(ESkillTreeResourceType ResourceType, const FGameplayTag& ResourceName) const
	{
		switch (ResourceType)
		{
			case ESkillTreeResourceType::Boolean:
				return BoolResources.Contains(ResourceName);
			case ESkillTreeResourceType::Scalar:
				return ScalarResources.Contains(ResourceName);
			case ESkillTreeResourceType::Integer:
				return IntegerResources.Contains(ResourceName);
			case ESkillTreeResourceType::Structure:
				return StructResources.Contains(ResourceName);
		}
		return false;
	}
	
	void RemoveResource(ESkillTreeResourceType ResourceType, const FGameplayTag& ResourceName)
	{
		switch (ResourceType)
		{
			case ESkillTreeResourceType::Boolean:
				BoolResources.Remove(ResourceName);
				break;
			case ESkillTreeResourceType::Scalar:
				ScalarResources.Remove(ResourceName);
				break;
			case ESkillTreeResourceType::Integer:
				IntegerResources.Remove(ResourceName);
				break;
			case ESkillTreeResourceType::Structure:
				StructResources.Remove(ResourceName);
				break;
		}
	}
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSet<FGameplayTag> BoolResources;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, float> ScalarResources;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, int32> IntegerResources;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ForceInlineRow))
	TMap<FGameplayTag, FInstancedStruct> StructResources;
	
};
