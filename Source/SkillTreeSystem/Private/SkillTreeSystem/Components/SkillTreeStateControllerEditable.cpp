// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Components/SkillTreeStateControllerEditable.h"

#include "SkillTreeSystem/Interfaces/SkillTreeSourceInterface.h"

void USkillTreeStateControllerEditable::InitTreeWithSource(const FName& TreeCategory, TScriptInterface<ISkillTreeSourceInterface> Source)
{
	if (!(Source && IsValid(Source.GetObject()))) return;
	
	TArray<FName> Nodes;
	ISkillTreeSourceInterface::Execute_GetNodesIds(Source.GetObject(), TreeCategory, Nodes);
	TArray<FSkillTreeLinkName> Links;
	ISkillTreeSourceInterface::Execute_GetLinks(Source.GetObject(), TreeCategory, Links);
	
	auto& TreeState = TreeStates.FindOrAdd(TreeCategory);
	for (const FName& NodeId : Nodes)
		TreeState.NodeStates.Add(NodeId);
	for (const FSkillTreeLinkName& Link : Links)
		TreeState.LinkStates.Add(Link);
}

void USkillTreeStateControllerEditable::GetNodeState(const FName& TreeCategory, const FName& NodeId, FSkillTreeNodeState& OutState)
{
	if (auto* TreeState = TreeStates.Find(TreeCategory))
	{
		if (auto* NodeState = TreeState->NodeStates.Find(NodeId))
		{
			OutState = *NodeState;
		}
	}
}

void USkillTreeStateControllerEditable::GetLinkState(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkState& OutState)
{
	if (auto* TreeState = TreeStates.Find(TreeCategory))
	{
		if (auto* LinkState = TreeState->LinkStates.Find(LinkName))
		{
			OutState = *LinkState;
		}
	}
}

void USkillTreeStateControllerEditable::SetNodeState(const FName& TreeCategory, const FName& NodeId, const FSkillTreeNodeState& InState)
{
	auto* TreeState = TreeStates.Find(TreeCategory);
	
	if (!TreeState) return;
	
	if (auto* Node = TreeState->NodeStates.Find(NodeId))
	{
		*Node = InState;
		OnSkillTreeNodeUpdated.Broadcast(TreeCategory, NodeId, InState);
	}
	
	for (auto& [LinkName, LinkState] : TreeState->LinkStates)
	{
		if (LinkName.StartNodeName == NodeId || LinkName.EndNodeName == NodeId)
		{
			const FSkillTreeNodeState* StartState = TreeState->NodeStates.Find(LinkName.StartNodeName);
			const FSkillTreeNodeState* EndState = TreeState->NodeStates.Find(LinkName.EndNodeName);
			
			if (StartState && EndState)
			{
				LinkState.InitFromNodeStates(*StartState, *EndState);
				OnSkillTreeLinkUpdated.Broadcast(TreeCategory, LinkName, LinkState);
			}
		}
	}
}
