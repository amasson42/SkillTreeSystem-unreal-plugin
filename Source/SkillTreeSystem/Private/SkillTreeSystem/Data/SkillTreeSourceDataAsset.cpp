// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Data/SkillTreeSourceDataAsset.h"

void USkillTreeSourceDataAsset::GetNodesIds_Implementation(const FName& TreeId, TArray<FName>& OutNodes)
{
	Nodes.GetKeys(OutNodes);
}

void USkillTreeSourceDataAsset::GetNodeClass_Implementation(const FName& TreeId, const FName& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass)
{
	OutWidgetClass = NodeWidgetClass;
}

void USkillTreeSourceDataAsset::GetNodeAppearance_Implementation(const FName& TreeId, const FName& NodeId, FSkillTreeNodeAppearance& OutAppearance)
{
	if (FSkillTreeSourceDataAssetElement* Element = Nodes.Find(NodeId))
	{
		OutAppearance = Element->Appearance;
	}
}

void USkillTreeSourceDataAsset::GetNodesPosition_Implementation(const FName& TreeCategory, TMap<FName, FSkillTreeNodePosition>& NodePositions)
{
	for (const auto& [NodeId, NodeData] : Nodes)
	{
		NodePositions.Add(NodeId, NodeData.Position);
	}
}

void USkillTreeSourceDataAsset::GetLinks_Implementation(const FName& TreeCategory, TArray<FSkillTreeLinkName>& OutLinks)
{
	for (const auto& [NodeId, NodeData] : Nodes)
	{
		for (const auto& [LinkedNode, _] : NodeData.Links)
		{
			OutLinks.Add({NodeId, LinkedNode});
		}
	}
}

void USkillTreeSourceDataAsset::GetLinkClass_Implementation(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass)
{
	OutWidgetClass = LinkWidgetClass;
}

void USkillTreeSourceDataAsset::GetLinkAppearance_Implementation(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkAppearance& OutAppearance)
{
	if (auto* StartNode = Nodes.Find(LinkName.StartNodeName))
	{
		if (auto* EndNodeAppearance = StartNode->Links.Find(LinkName.EndNodeName))
		{
			OutAppearance = *EndNodeAppearance;
		}
	}
}

void USkillTreeSourceDataAsset::GetLinksPositions_Implementation(const FName& TreeCategory, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinksPositions)
{
	for (const auto& [StartNodeId, StartNodeData] : Nodes)
	{
		for (const auto& [EndNodeId, _] : StartNodeData.Links)
		{
			FSkillTreeLinkPosition Position;
			Position.CanvasStartLocation = StartNodeData.Position.CanvasLocation;
			if (auto* EndNodeData = Nodes.Find(EndNodeId))
			{
				Position.CanvasEndLocation = EndNodeData->Position.CanvasLocation;
			}
			OutLinksPositions.Add({StartNodeId, EndNodeId}, Position);
		}
	}
}
