// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Widgets/SkillTreeWidget.h"

#include "SkillTreeSystem/StateController/SkillTreeStateControllerBase.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeSourceInterface.h"


void USkillTreeWidget::SetSource(TScriptInterface<ISkillTreeSourceInterface> InSource, const FGameplayTag& InTreeCategory)
{
	Source = InSource;
	TreeCategory = InTreeCategory;
	
	OnSourceUpdatedCheck();
}

void USkillTreeWidget::SetStateController(USkillTreeStateControllerBase* InStateController)
{
	ClearStateController();
	
	StateController = InStateController;
	
	if (!IsValid(InStateController))
		return;
	
	TArray<FGameplayTag> Nodes;
	TArray<FSkillTreeLinkName> Links;

	if (Source && IsValid(Source.GetObject()))
	{
		ISkillTreeSourceInterface::Execute_GetNodesIds(Source.GetObject(), TreeCategory, Nodes);	
		ISkillTreeSourceInterface::Execute_GetLinks(Source.GetObject(), TreeCategory, Links);
	}
	
	for (const FGameplayTag& NodeId : Nodes)
	{
		FSkillTreeNodeState NodeState;
		InStateController->GetNodeState(TreeCategory, NodeId, NodeState);
		
		OnNodeStateUpdated(NodeId, NodeState);
	}
	
	InStateController->OnSkillTreeNodeUpdated.AddDynamic(this, &ThisClass::OnSkillTreeNodeUpdated);
	
	for (const FSkillTreeLinkName& Link : Links)
	{
		FSkillTreeLinkState LinkState;
		InStateController->GetLinkState(TreeCategory, Link, LinkState);

		OnLinkStateUpdated(Link, LinkState);
	}
	
	InStateController->OnSkillTreeLinkUpdated.AddDynamic(this, &ThisClass::OnSkillTreeLinkUpdated);
}

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	OnSourceUpdatedCheck();
}

void USkillTreeWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ClearStateController();
}

#if WITH_EDITOR
void USkillTreeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	OnSourceUpdatedCheck();
}
#endif

void USkillTreeWidget::ClearStateController()
{
	if (StateController.IsValid())
	{
		auto* Controller = StateController.Get();
		Controller->OnSkillTreeNodeUpdated.RemoveAll(this);
		Controller->OnSkillTreeLinkUpdated.RemoveAll(this);
	}
	StateController.Reset();
}

void USkillTreeWidget::OnSkillTreeNodeUpdated(
	USkillTreeStateControllerBase* Controller,
	const FGameplayTag& InTreeCategory,
	const FGameplayTag& NodeId,
	const FSkillTreeNodeState& State)
{
	check(Controller == StateController);
	if (InTreeCategory == TreeCategory)
	{
		OnNodeStateUpdated(NodeId, State);
	}
}

void USkillTreeWidget::OnSkillTreeLinkUpdated(
	USkillTreeStateControllerBase* Controller,
	const FGameplayTag& InTreeCategory,
	const FSkillTreeLinkName& LinkName,
	const FSkillTreeLinkState& State)
{
	check(Controller == StateController);
	if (InTreeCategory == TreeCategory)
	{
		OnLinkStateUpdated(LinkName, State);
	}
}

void USkillTreeWidget::OnSourceUpdatedCheck()
{
	bool bValidSource = Source && IsValid(Source.GetObject());
	
	OnSourceUpdated(bValidSource, Source, TreeCategory);
}
