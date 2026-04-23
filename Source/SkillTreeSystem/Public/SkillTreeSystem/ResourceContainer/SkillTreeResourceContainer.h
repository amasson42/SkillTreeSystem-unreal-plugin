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
	
	template <typename T>
	void SetStructResource(const FGameplayTag& ResourceName, const T& Value)
	{
		StructResources.Add(ResourceName, FInstancedStruct::Make(Value));
	}
	
	template <typename T>
	const T* GetStructResource(const FGameplayTag& ResourceName) const
	{
		const FInstancedStruct* Entry = StructResources.Find(ResourceName);
		if (!Entry) return nullptr;
		return Entry->GetPtr<T>();
	}
	
	template <typename T>
	T* GetStructResourceMutable(const FGameplayTag& ResourceName)
	{
		FInstancedStruct* Entry = StructResources.Find(ResourceName);
		if (!Entry) return nullptr;
		return Entry->GetMutablePtr<T>();
	}
	
private:
	
	friend class USkillTreeResourceContainerLibrary;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSet<FGameplayTag> BoolResources;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, float> ScalarResources;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, int32> IntegerResources;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ForceInlineRow))
	TMap<FGameplayTag, FInstancedStruct> StructResources;
	
};
