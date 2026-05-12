// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/CanvasSource/SkillTreeCanvasSourceDataAsset.h"

#include "SkillTreeSystem/CanvasSource/LayoutMaker/SkillTreeLayoutMaker.h"


void USkillTreeCanvasSourceDataAsset::GetCanvasNodesIds_Implementation(TArray<FGameplayTag>& OutNodes)
{
	Nodes.GetKeys(OutNodes);
}

void USkillTreeCanvasSourceDataAsset::GetCanvasNodeData_Implementation(const FGameplayTag& NodeId,
	TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass, FSkillTreeNodeAppearance& OutAppearance)
{
	OutWidgetClass = NodeWidgetClass;
	if (const auto* Element = Nodes.Find(NodeId))
	{
		OutAppearance = Element->Appearance;
	}
}

void USkillTreeCanvasSourceDataAsset::GetCanvasLinkIds_Implementation(TArray<FSkillTreeLinkName>& OutLinks)
{
	if (!(LayoutSource && IsValid(LayoutSource.GetObject()))) return;
	
	TMap<FGameplayTag, FSkillTreeNodePosition> NodePositions;
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition> LinksPositions;
	ISkillTreeLayoutMaker::Execute_GetCanvasElementsPositions(LayoutSource.GetObject(), NodePositions, LinksPositions);
	
	LinksPositions.GetKeys(OutLinks);
}

void USkillTreeCanvasSourceDataAsset::GetCanvasLinkData_Implementation(const FSkillTreeLinkName& LinkName,
	TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass, FSkillTreeLinkAppearance& OutAppearance)
{
	OutWidgetClass = LinkWidgetClass;
	OutAppearance = LinkAppearance;
}

void USkillTreeCanvasSourceDataAsset::GetCanvasElementsPositions_Implementation(
	TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions,
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions)
{
	if (!(LayoutSource && IsValid(LayoutSource.GetObject()))) return;
	
	ISkillTreeLayoutMaker::Execute_GetCanvasElementsPositions(LayoutSource.GetObject(),
		OutNodePositions, OutLinkPositions);
}
