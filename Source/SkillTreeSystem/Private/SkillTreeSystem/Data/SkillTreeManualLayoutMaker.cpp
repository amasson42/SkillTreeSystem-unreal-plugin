// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Data/SkillTreeManualLayoutMaker.h"

void USkillTreeManualLayoutMaker::GetElementsPosition_Implementation(
	TMap<FName, FSkillTreeNodePosition>& NodePositions,
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& LinkPositions)
{
	for (const auto& [NodeId, NodeData] : Nodes)
	{
		NodePositions.Add(NodeId, NodeData.Position);
		
		for (const auto& Link : NodeData.Links)
		{
			if (auto* LinkData = Nodes.Find(Link))
			{
				FSkillTreeLinkPosition LinkPosition;
				LinkPosition.CanvasStartLocation = NodeData.Position.CanvasLocation;
				LinkPosition.CanvasEndLocation = LinkData->Position.CanvasLocation;
				LinkPositions.Add({NodeId, Link}, LinkPosition);
			}
		}
	}
}
