// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"
#include "UObject/Object.h"
#include "SkillTreeStateControllerBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSkillTreeNodeUpdated, USkillTreeStateControllerBase*, Controller, const FGameplayTag&, TreeCategory, const FGameplayTag&, NodeId, const FSkillTreeNodeState&, NodeState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSkillTreeLinkUpdated, USkillTreeStateControllerBase*, Controller, const FGameplayTag&, TreeCategory, const FSkillTreeLinkName&, LinkName, const FSkillTreeLinkState&, LinkState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeResourceUpdated, USkillTreeStateControllerBase*, Controller, ESkillTreeResourceType, ResourceType, const FGameplayTag&, ResourceName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeModifySkillRequest, USkillTreeStateControllerBase*, Controller, const FGameplayTag&, TreeCategory, const FGameplayTag&, NodeId);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class SKILLTREESYSTEM_API USkillTreeStateControllerBase : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual void GetNodeState(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, FSkillTreeNodeState& OutState) const {}
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual void GetLinkState(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState) const {}
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual const FSkillTreeResourceContainer& GetResourceContainer() const;
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeNodeUpdated OnSkillTreeNodeUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeLinkUpdated OnSkillTreeLinkUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeResourceUpdated OnSkillTreeResourceUpdated;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SkillTreeController|Request")
	void RequestIncreaseSkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId);
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|Request")
	FOnSkillTreeModifySkillRequest OnRequestIncreaseSkill;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SkillTreeController|Request")
	void RequestDecreaseSkill(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId);
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|Request")
	FOnSkillTreeModifySkillRequest OnRequestDecreaseSkill;
	
};
