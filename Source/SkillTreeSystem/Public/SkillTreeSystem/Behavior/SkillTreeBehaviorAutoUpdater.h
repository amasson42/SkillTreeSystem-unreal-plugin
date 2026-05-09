// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/ResourceContainer/SkillTreeResourceContainer.h"
#include "SkillTreeBehaviorAutoUpdater.generated.h"

class USkillTreeStateControllerEditable;
class USkillTreeStateControllerBase;
class ISkillTreeBehaviorInterface;

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTREESYSTEM_API USkillTreeBehaviorAutoUpdater : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillTree|Observing")
	bool ObserveControllerWithBehaviors(
		USkillTreeStateControllerEditable* Controller,
		const TArray<TScriptInterface<ISkillTreeBehaviorInterface>>& Behaviors);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTree|Observing")
	void StopObservingController();
	
private:
	
	void GatherInterestsForNode(UObject* Behavior, const FGameplayTag& NodeId);
	void MarkDirtyNode(const TWeakObjectPtr<UObject>& Behavior, const FGameplayTag& NodeId);
	void ScheduleUpdate();
	void UpdateDirtyNodes();
	
	UFUNCTION()
	void OnStateNodeUpdated(
		USkillTreeStateControllerBase* Controller,
		const FGameplayTag& NodeId,
		const FSkillTreeNodeState& NodeState);
	
	UFUNCTION()
	void OnStateResourceUpdated(
		USkillTreeStateControllerBase* Controller,
		ESkillTreeResourceType ResourceType,
		const FGameplayTag& ResourceName);
	
	TWeakObjectPtr<USkillTreeStateControllerEditable> ObservedController;
	
	TMap<
		FGameplayTag /* SkillName */,
		TSet<TPair<TWeakObjectPtr<UObject> /* Observed Behavior */, FGameplayTag /* Interested skills */>>> SkillInterestedNodes;
	TMap<
		TPair<ESkillTreeResourceType, FGameplayTag /* ResourceName */>,
		TSet<TPair<TWeakObjectPtr<UObject> /* Observed Behavior */, FGameplayTag /* Interested skills */>>> ResourceInterestedNodes;
	
	TSet<TPair<TWeakObjectPtr<UObject> /* Observed Behavior */, FGameplayTag>> DirtyNodes;
	
	bool bUpdatedScheduled = false;
	
};
