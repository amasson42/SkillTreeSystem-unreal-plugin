// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Data/SkillTreeCircularLayoutMaker.h"


struct _NodeData
{
	FGameplayTag Name;
	_NodeData* ParentPtr = nullptr;
	TArray<_NodeData*> ChildsPtr;
	
	int32 Level = -1;
	float Angle = 0.0f;
	float AngleRangeMin = 0.0f;
	float AngleRangeMax = 360.0f;
	
	FVector2D Position = FVector2D::ZeroVector;
	
	_NodeData(FGameplayTag InName): Name(InName) {}
};

static void _CalculateLocalNodeLevel(_NodeData* NodePtr)
{
	if (NodePtr->ParentPtr->Level < 0)
	{
		NodePtr->ParentPtr->Level = 0;
		
		if (NodePtr->ParentPtr->ParentPtr)
			_CalculateLocalNodeLevel(NodePtr->ParentPtr);
	}
	
	NodePtr->Level = NodePtr->ParentPtr->Level + 1;
};

static void _PopulateOutElements(
	const TMap<FGameplayTag, _NodeData>& LocalNodeDatas,
	TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
	for (const auto& [NodeName, LocalNodeData] : LocalNodeDatas)
	{
		FSkillTreeNodePosition NodePosition;
		NodePosition.CanvasLocation = LocalNodeData.Position;
		OutNodePositions.Add(NodeName, NodePosition);
		
		if (LocalNodeData.ParentPtr)
		{
			FSkillTreeLinkPosition LinkPosition;
			LinkPosition.CanvasStartLocation = LocalNodeData.ParentPtr->Position;
			LinkPosition.CanvasEndLocation = LocalNodeData.Position;
			
			OutLinkPositions.Add(
				{LocalNodeData.ParentPtr->Name, NodeName},
				LinkPosition
			);
		}
	}
}

void _CalculateNodesAngles(_NodeData* ParentNodeData, TSet<_NodeData*> Parents)
{
	const float ArcStartAngle = ParentNodeData->AngleRangeMin;
	const float ArcEndAngle = ParentNodeData->AngleRangeMax;
	const float TotalAngle = ArcEndAngle - ArcStartAngle;
	
	const int32 ChildCount = ParentNodeData->ChildsPtr.Num();
	const float SharedAngleLength = TotalAngle / FMath::Max(1, ChildCount);
	
	for (int32 i = 0; i < ChildCount; i++)
	{
		auto* Node = ParentNodeData->ChildsPtr[i];
		Node->AngleRangeMin = ArcStartAngle + i * SharedAngleLength;
		Node->AngleRangeMax = ArcStartAngle + (i + 1) * SharedAngleLength;
		Node->Angle = (Node->AngleRangeMin + Node->AngleRangeMax) / 2;
		if (!Parents.Contains(Node))
		{
			Parents.Add(Node);
			_CalculateNodesAngles(Node, Parents);
		}
	}
}

void USkillTreeCircularLayoutMaker::GetElementsPosition_Implementation(
	TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
	TMap<FGameplayTag, _NodeData> LocalNodeDatas;
	
	// Instanciate all local data
	for (const auto& Node : Nodes)
		LocalNodeDatas.Add(Node.Name, {Node.Name});
	
	// Link to parents and childrens
	for (const auto& Node : Nodes)
	{
		if (!Node.Parent.IsValid()) continue;
		
		auto& LocalNodeData = LocalNodeDatas[Node.Name];
		LocalNodeData.ParentPtr = LocalNodeDatas.Find(Node.Parent);
		if (LocalNodeData.ParentPtr)
			LocalNodeData.ParentPtr->ChildsPtr.Add(&LocalNodeData);
	}
	
	_NodeData RootNode {FGameplayTag()};
	
	// Find roots and levels
	for (auto& [NodeId, LocalNodeData] : LocalNodeDatas)
	{
		if (!LocalNodeData.ParentPtr)
		{
			RootNode.ChildsPtr.Add(&LocalNodeData);
			LocalNodeData.Level = 0;
			continue;
		}
		
		_CalculateLocalNodeLevel(&LocalNodeData);
	}
	
	// Calculate min and max angles
	RootNode.AngleRangeMin = ArcStartAngle;
	RootNode.AngleRangeMax = ArcEndAngle;
	
	TSet<_NodeData*> Parents;
	_CalculateNodesAngles(&RootNode, Parents);
	
	for (auto& [NodeId, NodeData] : LocalNodeDatas)
	{
		float RadAngle = FMath::DegreesToRadians(NodeData.Angle);
		float XDir = FMath::Cos(RadAngle);
		float YDir = FMath::Sin(RadAngle);
		float Distance = RootCircleRadius + NodeData.Level * CirclesDistance;
		
		NodeData.Position.X = XDir * Distance;
		NodeData.Position.Y = YDir * Distance;
	}
	
	_PopulateOutElements(LocalNodeDatas, OutNodePositions, OutLinkPositions);
}
