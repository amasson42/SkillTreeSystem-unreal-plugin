// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Structures/SkillTreeCoreStructures.h"
#include "UObject/Object.h"
#include "SkillTreeStateControllerBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeNodeUpdated, const FName&, TreeCategory, const FName&, NodeId, const FSkillTreeNodeState&, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeLinkUpdated, const FName&, TreeCategory, const FSkillTreeLinkName&, LinkName, const FSkillTreeLinkState&, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeRequestModify, const FName&, TreeCategory, const FName&, NodeId, int32, NewLevel);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class SKILLTREESYSTEM_API USkillTreeStateControllerBase : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual void GetNodeState(const FName& TreeCategory, const FName& NodeId, FSkillTreeNodeState& OutState) {}
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|State")
	virtual void GetLinkState(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState) {}
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeNodeUpdated OnSkillTreeNodeUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|State")
	FOnSkillTreeLinkUpdated OnSkillTreeLinkUpdated;
	
	UFUNCTION(BlueprintCallable, Category = "SkillTreeController|Request")
	void RequestModifySkill(const FName& TreeCategory, const FName& NodeId, int32 NewLevel);
	
	UPROPERTY(BlueprintAssignable, Category = "SkillTreeController|Request")
	FOnSkillTreeRequestModify OnRequestPickSkill;
	
};
