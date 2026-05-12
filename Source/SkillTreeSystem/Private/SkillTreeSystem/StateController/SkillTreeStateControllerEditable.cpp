// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/StateController/SkillTreeStateControllerEditable.h"

#include "SkillTreeSystem/CanvasSource/SkillTreeCanvasSourceInterface.h"

void USkillTreeStateControllerEditable::InitTreeWithSource(TScriptInterface<ISkillTreeCanvasSourceInterface> Source)
{
	if (!(Source && IsValid(Source.GetObject()))) return;
	
	TArray<FGameplayTag> Nodes;
	ISkillTreeCanvasSourceInterface::Execute_GetCanvasNodesIds(Source.GetObject(), Nodes);
	TArray<FSkillTreeLinkName> Links;
	ISkillTreeCanvasSourceInterface::Execute_GetCanvasLinkIds(Source.GetObject(), Links);
	
	for (const FGameplayTag& NodeId : Nodes)
		NodeStates.Add(NodeId);
	for (const FSkillTreeLinkName& Link : Links)
		LinkStates.Add(Link);
}

void USkillTreeStateControllerEditable::GetNodeState(const FGameplayTag& NodeId, FSkillTreeNodeState& OutState) const
{
	if (auto* NodeState = NodeStates.Find(NodeId))
	{
		OutState = *NodeState;
	}
}

void USkillTreeStateControllerEditable::GetLinkState(const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState) const
{
	if (auto* LinkState = LinkStates.Find(LinkName))
	{
		OutState = *LinkState;
	}
}

const FSkillTreeResourceContainer& USkillTreeStateControllerEditable::GetResourceContainer() const
{
	return ResourceContainer;
}

void USkillTreeStateControllerEditable::SetNodeState(const FGameplayTag& NodeId, const FSkillTreeNodeState& InState)
{
	auto* NodeState = NodeStates.Find(NodeId);
	if (!NodeState) return;
	
	if (*NodeState == InState) return;
	
	*NodeState = InState;
	OnSkillTreeNodeUpdated.Broadcast(this, NodeId, InState);
	
	for (auto& [LinkName, LinkState] : LinkStates)
	{
		if (LinkName.StartNodeName == NodeId || LinkName.EndNodeName == NodeId)
		{
			const FSkillTreeNodeState* StartState = NodeStates.Find(LinkName.StartNodeName);
			const FSkillTreeNodeState* EndState = NodeStates.Find(LinkName.EndNodeName);
			
			if (StartState && EndState)
			{
				if (LinkState.SetFromNodeStates(*StartState, *EndState))
					OnSkillTreeLinkUpdated.Broadcast(this, LinkName, LinkState);
			}
		}
	}
}

void USkillTreeStateControllerEditable::SetNodeLevel(const FGameplayTag& NodeId, int32 NewLevel)
{
	FSkillTreeNodeState State;
	GetNodeState(NodeId, State);
	State.Level = NewLevel;
	SetNodeState(NodeId, State);
}

void USkillTreeStateControllerEditable::SetBoolResource(FGameplayTag ResourceName, bool Value)
{
	ResourceContainer.SetBoolResource(ResourceName, Value);
	OnSkillTreeResourceUpdated.Broadcast(this, ESkillTreeResourceType::Boolean, ResourceName);
}

void USkillTreeStateControllerEditable::SetScalarResource(FGameplayTag ResourceName, float Value)
{
	ResourceContainer.SetScalarResource(ResourceName, Value);
	OnSkillTreeResourceUpdated.Broadcast(this, ESkillTreeResourceType::Scalar, ResourceName);
}

void USkillTreeStateControllerEditable::SetIntegerResource(FGameplayTag ResourceName, int32 Value)
{
	ResourceContainer.SetIntegerResource(ResourceName, Value);
	OnSkillTreeResourceUpdated.Broadcast(this, ESkillTreeResourceType::Integer, ResourceName);
}

void USkillTreeStateControllerEditable::SetStructResource(FGameplayTag ResourceName, const FInstancedStruct& Value)
{
	ResourceContainer.SetStructResource(ResourceName, Value);
	OnSkillTreeResourceUpdated.Broadcast(this, ESkillTreeResourceType::Structure, ResourceName);
}
