// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Structures/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"
#include "UObject/Object.h"
#include "SkillTreeStateControllerBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeNodeUpdated, const FGameplayTag&, TreeCategory, const FGameplayTag&, NodeId, const FSkillTreeNodeState&, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeLinkUpdated, const FGameplayTag&, TreeCategory, const FSkillTreeLinkName&, LinkName, const FSkillTreeLinkState&, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillTreeResourceUpdated, ESkillTreeResourceType, ResourceType, const FGameplayTag&, ResourceName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeRequestModify, const FGameplayTag&, TreeCategory, const FGameplayTag&, NodeId, int32, NewLevel);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class SKILLTREESYSTEM_API USkillTreeStateControllerBase : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual void GetNodeState(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, FSkillTreeNodeState& OutState) {}
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual void GetLinkState(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState) {}
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual const FSkillTreeResourceContainer& GetResourceContainer();
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeNodeUpdated OnSkillTreeNodeUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeLinkUpdated OnSkillTreeLinkUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeResourceUpdated OnSkillTreeResourceUpdated;
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|Request")
	void RequestModifySkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, int32 NewLevel);
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|Request")
	FOnSkillTreeRequestModify OnRequestPickSkill;
	
};
