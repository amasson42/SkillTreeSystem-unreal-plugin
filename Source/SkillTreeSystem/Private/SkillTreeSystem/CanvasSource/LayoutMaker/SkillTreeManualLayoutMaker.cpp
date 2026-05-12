// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/CanvasSource/LayoutMaker/SkillTreeManualLayoutMaker.h"

void USkillTreeManualLayoutMaker::GetCanvasElementsPositions_Implementation(
	TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
	for (const auto& [NodeId, NodeData] : Nodes)
	{
		OutNodePositions.Add(NodeId, NodeData.Position);
		
		for (const auto& Link : NodeData.Links)
		{
			if (auto* LinkData = Nodes.Find(Link))
			{
				FSkillTreeLinkPosition LinkPosition;
				LinkPosition.CanvasStartLocation = NodeData.Position.CanvasLocation;
				LinkPosition.CanvasEndLocation = LinkData->Position.CanvasLocation;
				OutLinkPositions.Add({NodeId, Link}, LinkPosition);
			}
		}
	}
}
