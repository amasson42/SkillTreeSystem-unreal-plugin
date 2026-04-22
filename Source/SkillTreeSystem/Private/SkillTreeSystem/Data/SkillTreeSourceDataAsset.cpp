// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Data/SkillTreeSourceDataAsset.h"

#include "SkillTreeSystem/Interfaces/SkillTreeLayoutMaker.h"

void USkillTreeSourceDataAsset::GetNodesIds_Implementation(const FGameplayTag& TreeCategory, TArray<FGameplayTag>& OutNodes)
{
	if (const auto* Tree = Trees.Find(TreeCategory))
	{
		Tree->Nodes.GetKeys(OutNodes);
	}
}

void USkillTreeSourceDataAsset::GetNodeClass_Implementation(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass)
{
	if (const auto* Tree = Trees.Find(TreeCategory))
	{
		OutWidgetClass = Tree->NodeWidgetClass;
	}
}

void USkillTreeSourceDataAsset::GetNodeAppearance_Implementation(const FGameplayTag& TreeCategory, const FGameplayTag& NodeId, FSkillTreeNodeAppearance& OutAppearance)
{
	if (const auto* Tree = Trees.Find(TreeCategory))
	{
		if (const auto* Element = Tree->Nodes.Find(NodeId))
		{
			OutAppearance = Element->Appearance;
		}
	}
}

void USkillTreeSourceDataAsset::GetNodesPosition_Implementation(const FGameplayTag& TreeCategory, TMap<FGameplayTag, FSkillTreeNodePosition>& NodePositions)
{
	const auto* Tree = Trees.Find(TreeCategory);
	if (!Tree) return;
	
	if (!(Tree->LayoutSource && IsValid(Tree->LayoutSource.GetObject()))) return;
	
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition> LinksPositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(Tree->LayoutSource.GetObject(), NodePositions, LinksPositions);
}

void USkillTreeSourceDataAsset::GetLinks_Implementation(const FGameplayTag& TreeCategory, TArray<FSkillTreeLinkName>& OutLinks)
{
	const auto* Tree = Trees.Find(TreeCategory);
	if (!Tree) return;
	
	if (!(Tree->LayoutSource && IsValid(Tree->LayoutSource.GetObject()))) return;
	
	TMap<FGameplayTag, FSkillTreeNodePosition> NodePositions;
	TMap<FSkillTreeLinkName, FSkillTreeLinkPosition> LinksPositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(Tree->LayoutSource.GetObject(), NodePositions, LinksPositions);
	
	LinksPositions.GetKeys(OutLinks);
}

void USkillTreeSourceDataAsset::GetLinkClass_Implementation(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass)
{
	if (const auto* Tree = Trees.Find(TreeCategory))
	{
		OutWidgetClass = Tree->LinkWidgetClass;
	}
}

void USkillTreeSourceDataAsset::GetLinkAppearance_Implementation(const FGameplayTag& TreeCategory, const FSkillTreeLinkName& LinkName, FSkillTreeLinkAppearance& OutAppearance)
{
	if (const auto* Tree = Trees.Find(TreeCategory))
	{
		OutAppearance = Tree->LinkAppearance;
	}
}

void USkillTreeSourceDataAsset::GetLinksPositions_Implementation(const FGameplayTag& TreeCategory, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinksPositions)
{
	const auto* Tree = Trees.Find(TreeCategory);
	if (!Tree) return;
	
	if (!(Tree->LayoutSource && IsValid(Tree->LayoutSource.GetObject()))) return;
	
	TMap<FGameplayTag, FSkillTreeNodePosition> NodePositions;
	ISkillTreeLayoutMaker::Execute_GetElementsPosition(Tree->LayoutSource.GetObject(), NodePositions, OutLinksPositions);
}
