// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/CanvasSource/SkillTreeCanvasSourceDataAsset.h"

#include "SkillTreeSystem/CanvasSource/LayoutMaker/SkillTreeLayoutMaker.h"


void USkillTreeCanvasSourceDataAsset::GetNodesIds_Implementation(TArray<FGameplayTag>& OutNodes)
{
	Nodes.GetKeys(OutNodes);
}

void USkillTreeCanvasSourceDataAsset::GetNodeClass_Implementation(const FGameplayTag& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass)
{
	OutWidgetClass = NodeWidgetClass;
}

void USkillTreeCanvasSourceDataAsset::GetNodeAppearance_Implementation(const FGameplayTag& NodeId, FSkillTreeNodeAppearance& OutAppearance)
{
	if (const auto* Element = Nodes.Find(NodeId))
	{
		OutAppearance = Element->Appearance;
	}
}

void USkillTreeCanvasSourceDataAsset::GetNodesPosition_Implementation(TMap<FGameplayTag, FSkillTreeNodePosition>& NodePositions)
{
	if (!(LayoutSource && IsValid(LayoutSource.GetObject()))) return;
	
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition> LinksPositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(LayoutSource.GetObject(), NodePositions, LinksPositions);
}

void USkillTreeCanvasSourceDataAsset::GetLinks_Implementation(TArray<FSkillTreeLinkName>& OutLinks)
{
	if (!(LayoutSource && IsValid(LayoutSource.GetObject()))) return;
	
	TMap<FGameplayTag, FSkillTreeNodePosition> NodePositions;
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition> LinksPositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(LayoutSource.GetObject(), NodePositions, LinksPositions);
	
	LinksPositions.GetKeys(OutLinks);
}

void USkillTreeCanvasSourceDataAsset::GetLinkClass_Implementation(const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass)
{
	OutWidgetClass = LinkWidgetClass;
}

void USkillTreeCanvasSourceDataAsset::GetLinkAppearance_Implementation(const FSkillTreeLinkName& LinkName, FSkillTreeLinkAppearance& OutAppearance)
{
	OutAppearance = LinkAppearance;
}

void USkillTreeCanvasSourceDataAsset::GetLinksPositions_Implementation(TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinksPositions)
{
	if (!(LayoutSource && IsValid(LayoutSource.GetObject()))) return;
	
	TMap<FGameplayTag, FSkillTreeNodePosition> NodePositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(LayoutSource.GetObject(), NodePositions, OutLinksPositions);
}
