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
	bool ObserveControllerWithBehavior(
		USkillTreeStateControllerEditable* Controller,
		TScriptInterface<ISkillTreeBehaviorInterface> Behavior,
		const TArray<FGameplayTag>& NodeIds);
	
	UFUNCTION(BlueprintCallable, Category = "SkillTree|Observing")
	void StopObservingController();
	
private:
	
	void GatherInterestsForNode(const FGameplayTag& NodeId);
	void MarkDirtyNode(const FGameplayTag& NodeId);
	void ScheduleUpdate();
	void UpdateDirtyNodes();
	
	UFUNCTION()
	void OnStateNodeUpdated(
		USkillTreeStateControllerBase* Controller,
		const FGameplayTag& TreeCategory,
		const FGameplayTag& NodeId,
		const FSkillTreeNodeState& NodeState);
	
	UFUNCTION()
	void OnStateResourceUpdated(
		USkillTreeStateControllerBase* Controller,
		ESkillTreeResourceType ResourceType,
		const FGameplayTag& ResourceName);
	
	TWeakObjectPtr<USkillTreeStateControllerEditable> ObservedController;
	TWeakObjectPtr<UObject> ObservedBehavior;
	
	TMap<
		TPair<FGameplayTag /* TreeCategory */, FGameplayTag /* SkillName */>,
		TSet<FGameplayTag>> SkillInterestedNodes;
	TMap<
		TPair<ESkillTreeResourceType, FGameplayTag /* ResourceName */>,
		TSet<FGameplayTag>> ResourceInterestedNodes;
	
	TSet<FGameplayTag> DirtyNodes;
	
	bool bUpdatedScheduled = false;
	
};
