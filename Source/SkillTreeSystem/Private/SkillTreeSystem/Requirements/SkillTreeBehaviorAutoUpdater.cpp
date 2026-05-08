// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Requirements/SkillTreeBehaviorAutoUpdater.h"

#include "SkillTreeSystem/Components/SkillTreeStateControllerEditable.h"
#include "SkillTreeSystem/Interfaces/SkillTreeBehaviorInterface.h"


bool USkillTreeBehaviorAutoUpdater::ObserveControllerWithBehavior(
	USkillTreeStateControllerEditable* Controller,
	TScriptInterface<ISkillTreeBehaviorInterface> Behavior,
	const TArray<FGameplayTag>& NodeIds)
{
	if (!(Behavior && IsValid(Behavior.GetObject()))) return false;
	
	if (!IsValid(Controller)) return false;
	
	StopObservingController();
	
	ObservedController = Controller;
	ObservedBehavior = Behavior.GetObject();
	
	Controller->OnSkillTreeNodeUpdated.AddDynamic(this, &ThisClass::OnStateNodeUpdated);
	Controller->OnSkillTreeResourceUpdated.AddDynamic(this, &ThisClass::OnStateResourceUpdated);
	
	for (const auto& NodeId : NodeIds)
	{
		GatherInterestsForNode(NodeId);
		MarkDirtyNode(NodeId);
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

void USkillTreeBehaviorAutoUpdater::GatherInterestsForNode(const FGameplayTag& NodeId)
{
	if (!ObservedBehavior.IsValid()) return;
	
	auto* Behavior = ObservedBehavior.Get();
	
	FSkillTreeBehaviorInterest Interests;
	ISkillTreeBehaviorInterface::Execute_GatherInterestsForNode(Behavior, NodeId, Interests);
	
	for (const auto& SkillInterest : Interests.SkillInterest)
	{
		SkillInterestedNodes
			.FindOrAdd({SkillInterest.TreeName, SkillInterest.SkillName})
			.Add(NodeId);
	}
	for (const auto& ResourceInterest : Interests.ResourceInterests)
	{
		ResourceInterestedNodes
			.FindOrAdd({ResourceInterest.ResourceType, ResourceInterest.ResourceName})
			.Add(NodeId);
	}
}

void USkillTreeBehaviorAutoUpdater::MarkDirtyNode(const FGameplayTag& NodeId)
{
	DirtyNodes.Add(NodeId);
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
	
	if (!ObservedBehavior.IsValid()) return;
	auto* Behavior = ObservedBehavior.Get();
	
	if (!ObservedController.IsValid()) return;
	auto* Controller = ObservedController.Get();
	
	TSet<FGameplayTag> CachedDirtyNodes = DirtyNodes;
	for (const auto& DirtyNode : CachedDirtyNodes)
	{
		ISkillTreeBehaviorInterface::Execute_UpdateNodeState(Behavior, DirtyNode, Controller);
	}
	DirtyNodes.Reset();
}

void USkillTreeBehaviorAutoUpdater::OnStateNodeUpdated(
	USkillTreeStateControllerBase* Controller,
	const FGameplayTag& TreeCategory,
	const FGameplayTag& NodeId,
	const FSkillTreeNodeState& NodeState)
{
	MarkDirtyNode(NodeId);
	if (const TSet<FGameplayTag>* InterestedNodes = SkillInterestedNodes.Find({TreeCategory, NodeId}))
	{
		for (const FGameplayTag& InterestedNode : *InterestedNodes)
		{
			MarkDirtyNode(InterestedNode);
		}
	}
}

void USkillTreeBehaviorAutoUpdater::OnStateResourceUpdated(
	USkillTreeStateControllerBase* Controller,
	ESkillTreeResourceType ResourceType,
	const FGameplayTag& ResourceName)
{
	if (const TSet<FGameplayTag>* InterestedNodes = ResourceInterestedNodes.Find({ResourceType, ResourceName}))
	{
		for (const FGameplayTag& InterestedNode : *InterestedNodes)
		{
			MarkDirtyNode(InterestedNode);
		}
	}
}
