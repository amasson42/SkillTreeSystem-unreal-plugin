// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Data/SkillTreeSourceDataAsset.h"

#include "SkillTreeSystem/Interfaces/SkillTreeLayoutMaker.h"

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
	if (!(LayoutSource && IsValid(LayoutSource.GetObject())))
		return;

	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition> LinksPositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(LayoutSource.GetObject(), NodePositions, LinksPositions);
}

void USkillTreeSourceDataAsset::GetLinks_Implementation(const FName& TreeCategory, TArray<FSkillTreeLinkName>& OutLinks)
{
	if (!(LayoutSource && IsValid(LayoutSource.GetObject())))
		return;
	
	TMap<FName, FSkillTreeNodePosition> NodePositions;
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition> LinksPositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(LayoutSource.GetObject(), NodePositions, LinksPositions);
	
	LinksPositions.GetKeys(OutLinks);
}

void USkillTreeSourceDataAsset::GetLinkClass_Implementation(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass)
{
	OutWidgetClass = LinkWidgetClass;
}

void USkillTreeSourceDataAsset::GetLinkAppearance_Implementation(const FName& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkAppearance& OutAppearance)
{
	OutAppearance = LinkAppearance;
}

void USkillTreeSourceDataAsset::GetLinksPositions_Implementation(const FName& TreeCategory, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinksPositions)
{
	if (!(LayoutSource && IsValid(LayoutSource.GetObject())))
		return;

	TMap<FName, FSkillTreeNodePosition> NodePositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(LayoutSource.GetObject(), NodePositions, OutLinksPositions);
}
