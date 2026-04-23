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
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static bool GetBoolResource(const FSkillTreeResourceContainer& Container, FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void SetBoolResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag Tag, bool Value);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static float GetScalarResource(const FSkillTreeResourceContainer& Container, FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void SetScalarResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag Tag, float Value);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static int32 GetIntegerResource(const FSkillTreeResourceContainer& Container, FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	static void SetIntegerResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag Tag, int32 Value);
	
	/**
	 * Retrieve a resource from the container by tag.
	 *
	 * The type of the OutValue pin determines which resource type is fetched.
	 * Returns false if the tag is not present or the stored type does not
	 * match the pin type.
	 *
	 * Blueprint: connect any struct variable to OutValue to drive the type.
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Resources",
		meta = (CustomStructureParam = "OutValue",
				ExpandBoolAsExecs = "ReturnValue"))
	static bool GetStructResource(const FSkillTreeResourceContainer& Container, FGameplayTag Tag, int32& OutValue);
	
	/**
	 * Store a resource in the container by tag.
	 *
	 * The type of the InValue pin determines the stored type.
	 * Replaces any existing resource under that tag.
	 *
	 * Blueprint: connect any struct variable to InValue to drive the type.
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Resources",
		meta = (CustomStructureParam = "InValue"))
	static void SetStructResource(UPARAM(ref) FSkillTreeResourceContainer& Container, FGameplayTag Tag, const int32& InValue);
	
	DECLARE_FUNCTION(execGetStructResource)
    {
        // Read Container (struct passed by ref)
        P_GET_STRUCT_REF(FSkillTreeResourceContainer, Container);
 
        // Read Tag
        P_GET_STRUCT(FGameplayTag, Tag);
 
        // Read the wildcard output property + address
        Stack.StepCompiledIn<FStructProperty>(nullptr);
        void*            OutValueAddr = Stack.MostRecentPropertyAddress;
        FStructProperty* OutValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
 
        P_FINISH;
 
        P_NATIVE_BEGIN;
 
        bool bSuccess = false;
 
        if (OutValueAddr && OutValueProp)
        {
            const FInstancedStruct* Entry = Container.StructResources.Find(Tag);
            if (Entry && Entry->IsValid())
            {
                const UScriptStruct* StoredType  = Entry->GetScriptStruct();
                const UScriptStruct* RequestType = OutValueProp->Struct;
 
                // Allow exact match or derived types.
                if (StoredType && (StoredType == RequestType || StoredType->IsChildOf(RequestType)))
                {
                    RequestType->CopyScriptStruct(OutValueAddr, Entry->GetMemory());
                    bSuccess = true;
                }
            }
        }
 
        *(bool*)RESULT_PARAM = bSuccess;
 
        P_NATIVE_END;
    }
 
    DECLARE_FUNCTION(execSetStructResource)
    {
        // Read Container (struct passed by ref)
        P_GET_STRUCT_REF(FSkillTreeResourceContainer, Container);
 
        // Read Tag
        P_GET_STRUCT(FGameplayTag, Tag);
 
        // Read the wildcard input property + address
        Stack.StepCompiledIn<FStructProperty>(nullptr);
        const void*      InValueAddr = Stack.MostRecentPropertyAddress;
        FStructProperty* InValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
 
        P_FINISH;
 
        P_NATIVE_BEGIN;
 
        if (InValueAddr && InValueProp)
        {
            FInstancedStruct NewEntry;
            NewEntry.InitializeAs(InValueProp->Struct, static_cast<const uint8*>(InValueAddr));
            Container.StructResources.Add(Tag, MoveTemp(NewEntry));
        }
 
        P_NATIVE_END;
    }
};
