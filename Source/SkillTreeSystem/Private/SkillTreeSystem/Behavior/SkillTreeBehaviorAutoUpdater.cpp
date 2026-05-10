// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Behavior/SkillTreeBehaviorAutoUpdater.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerEditable.h"
#include "SkillTreeSystem/Behavior/SkillTreeBehaviorInterface.h"


bool USkillTreeBehaviorAutoUpdater::ObserveControllerWithBehaviors(
	USkillTreeStateControllerEditable* Controller,
	const TArray<TScriptInterface<ISkillTreeBehaviorInterface>>& Behaviors)
{
	if (!IsValid(Controller)) return false;
	
	StopObservingController();
	
	ObservedController = Controller;
	
	Controller->OnSkillTreeNodeUpdated.AddDynamic(this, &ThisClass::OnStateNodeUpdated);
	Controller->OnSkillTreeResourceUpdated.AddDynamic(this, &ThisClass::OnStateResourceUpdated);
	
	for (const auto& Behavior : Behaviors)
	{
		if (!(Behavior && IsValid(Behavior.GetObject()))) continue;
		
		TArray<FGameplayTag> NodeIds;
		ISkillTreeBehaviorInterface::Execute_GetNodesIds(Behavior.GetObject(), NodeIds);
		for (const auto& NodeId : NodeIds)
		{
			GatherInterestsForNode(Behavior.GetObject(), NodeId);
			MarkDirtyNode(Behavior.GetObject(), NodeId);
		}
	}
	
	return true;
}

void USkillTreeBehaviorAutoUpdater::StopObservingController()
{
	if (!ObservedController.IsValid()) return;
	auto* Controller = ObservedController.Get();
	
	Controller->OnSkillTreeNodeUpdated.RemoveDynamic(this, &ThisClass::OnStateNodeUpdated);
	Controller->OnSkillTreeResourceUpdated.RemoveDynamic(this, &ThisClass::OnStateResourceUpdated);
}

void USkillTreeBehaviorAutoUpdater::GatherInterestsForNode(UObject* Behavior, const FGameplayTag& NodeId)
{
	check(IsValid(Behavior) && Behavior->Implements<USkillTreeBehaviorInterface>());
	
	FSkillTreeBehaviorInterest Interests;
	ISkillTreeBehaviorInterface::Execute_GatherInterestsForNode(Behavior, NodeId, Interests);
	
	SkillInterestedNodes.FindOrAdd(NodeId).Add({Behavior, NodeId});
	
	for (const auto& SkillInterest : Interests.SkillInterest)
	{
		SkillInterestedNodes
			.FindOrAdd(SkillInterest.SkillName)
			.Add({Behavior, NodeId});
	}
	for (const auto& ResourceInterest : Interests.ResourceInterests)
	{
		ResourceInterestedNodes
			.FindOrAdd({ResourceInterest.ResourceType, ResourceInterest.ResourceName})
			.Add({Behavior, NodeId});
	}
}

void USkillTreeBehaviorAutoUpdater::MarkDirtyNode(const TWeakObjectPtr<UObject>& Behavior, const FGameplayTag& NodeId)
{
	DirtyNodes.Add({Behavior, NodeId});
	ScheduleUpdate();
}

void USkillTreeBehaviorAutoUpdater::ScheduleUpdate()
{
	if (bUpdatedScheduled) return;
	
	if (!ObservedController.IsValid()) return;
	
	UWorld* World = ObservedController->GetWorld();
	if (!World) return;
	
	bUpdatedScheduled = true;
	
	World->GetTimerManager().SetTimerForNextTick([this]()
	{
		bUpdatedScheduled = false;
		UpdateDirtyNodes();
	});
}

void USkillTreeBehaviorAutoUpdater::UpdateDirtyNodes()
{
	if (DirtyNodes.IsEmpty()) return;
	
	if (!ObservedController.IsValid()) return;
	auto* Controller = ObservedController.Get();
	
	auto CachedDirtyNodes = DirtyNodes;
	for (const auto& [BehaviorPtr, DirtyNode] : CachedDirtyNodes)
	{
		if (BehaviorPtr->IsValidLowLevel())
		{
			ISkillTreeBehaviorInterface::Execute_UpdateNodeState(BehaviorPtr.Get(), DirtyNode, Controller);
		}
	}
	DirtyNodes.Reset();
}

void USkillTreeBehaviorAutoUpdater::OnStateNodeUpdated(
	USkillTreeStateControllerBase* Controller,
	const FGameplayTag& NodeId,
	const FSkillTreeNodeState& NodeState)
{
	if (const auto* InterestedNodes = SkillInterestedNodes.Find(NodeId))
	{
		for (const auto& [Behavior, InterestedNode] : *InterestedNodes)
		{
			MarkDirtyNode(Behavior, InterestedNode);
		}
	}
}

void USkillTreeBehaviorAutoUpdater::OnStateResourceUpdated(
	USkillTreeStateControllerBase* Controller,
	ESkillTreeResourceType ResourceType,
	const FGameplayTag& ResourceName)
{
	if (const auto* InterestedNodes = ResourceInterestedNodes.Find({ResourceType, ResourceName}))
	{
		for (const auto& [Behavior, InterestedNode] : *InterestedNodes)
		{
			MarkDirtyNode(Behavior, InterestedNode);
		}
	}
}
