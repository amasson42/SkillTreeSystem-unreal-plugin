// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeSystem/Widgets/MaterialBlendTask.h"
#include "Components/Image.h"

UMaterialBlendTask* UMaterialBlendTask::BlendToMaterial(
	UObject* WorldContextObject,
	UImage* InTargetImage,
	UMaterialInterface* InNewMaterial,
	TSet<FName> ScalarParams,
	TSet<FName> VectorParams,
	float InBlendDuration,
	UCurveFloat* InBlendCurve)
{
	UMaterialBlendTask* Task = NewObject<UMaterialBlendTask>(WorldContextObject);

	Task->TargetImage = InTargetImage;
	Task->NewMaterial = InNewMaterial;
	Task->BlendDuration = FMath::Max(0.0f, InBlendDuration);
	Task->BlendCurve = InBlendCurve;

	for (const FName& Name : ScalarParams)
	{
		FScalarBlendParam& NewParam = Task->ScalarBlends.AddDefaulted_GetRef();
		NewParam.Name = Name;
	}

	for (const FName& Name : VectorParams)
	{
		FVectorBlendParam& NewParam = Task->VectorBlends.AddDefaulted_GetRef();
		NewParam.Name = Name;
	}

	return Task;
}

void UMaterialBlendTask::Activate()
{
	if (!TargetImage || !NewMaterial)
	{
		OnCancelled.Broadcast();
		SetReadyToDestroy();
		return;
	}

	UMaterialInterface* CurrentMaterial = nullptr;

	const FSlateBrush& Brush = TargetImage->GetBrush();
	if (UMaterialInstanceDynamic* ExistingDMI = Cast<UMaterialInstanceDynamic>(Brush.GetResourceObject()))
	{
		ActiveDMI = ExistingDMI;
		CurrentMaterial = ExistingDMI;
	}
	else if (UMaterialInterface* BrushMat = Cast<UMaterialInterface>(Brush.GetResourceObject()))
	{
		ActiveDMI = UMaterialInstanceDynamic::Create(BrushMat, this);
		CurrentMaterial = BrushMat;
		TargetImage->SetBrushFromMaterial(ActiveDMI);
	}
	else
	{
		ActiveDMI = UMaterialInstanceDynamic::Create(NewMaterial, this);
		TargetImage->SetBrushFromMaterial(ActiveDMI);
	}

	if (BlendDuration <= 0.0f)
	{
		FinaliseBlend();
		return;
	}

	for (auto& Param : ScalarBlends)
	{
		if (ActiveDMI)
			ActiveDMI->GetScalarParameterValue(Param.Name, Param.From);
		if (!NewMaterial->GetScalarParameterValue(Param.Name, Param.To))
			Param.To = 0.0f;
	}

	for (auto& Param : VectorBlends)
	{
		if (ActiveDMI)
			ActiveDMI->GetVectorParameterValue(Param.Name, Param.From);
		if (!NewMaterial->GetVectorParameterValue(Param.Name, Param.To))
			Param.To = FLinearColor::White;
	}
	
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UMaterialBlendTask::Tick));
}

void UMaterialBlendTask::Cancel()
{
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	OnCancelled.Broadcast();
	SetReadyToDestroy();
}

bool UMaterialBlendTask::Tick(float DeltaTime)
{
	if (!IsValid(TargetImage) || !IsValid(ActiveDMI))
	{
		Cancel();
		return false;
	}

	BlendElapsed += DeltaTime;

	float Time = FMath::Clamp(BlendElapsed / BlendDuration, 0.0f, 1.0f);
	float Alpha = BlendCurve ? BlendCurve->GetFloatValue(Time) : Time;

	ApplyBlendedValues(Alpha);
	OnUpdated.Broadcast();

	if (BlendElapsed > BlendDuration)
	{
		FinaliseBlend();
		return false;
	}

	return true;
}

void UMaterialBlendTask::ApplyBlendedValues(float Alpha)
{
	if (!ActiveDMI) return;

	for (const auto& Param : ScalarBlends)
	{
		float LerpValue = FMath::Lerp(Param.From, Param.To, Alpha);
		ActiveDMI->SetScalarParameterValue(Param.Name, LerpValue);
	}

	for (const auto& Param : VectorBlends)
	{
		FLinearColor LerpValue = FLinearColor::LerpUsingHSV(Param.From, Param.To, Alpha);
		ActiveDMI->SetVectorParameterValue(Param.Name, LerpValue);
	}
}

void UMaterialBlendTask::FinaliseBlend()
{
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	ApplyBlendedValues(1.0f);

	if (IsValid(TargetImage) && NewMaterial)
	{
		UMaterialInstanceDynamic* FinalDMI = UMaterialInstanceDynamic::Create(NewMaterial, this);
		if (FinalDMI)
		{
			for (const auto& Param : ScalarBlends)
				FinalDMI->SetScalarParameterValue(Param.Name, Param.To);
			for (const auto& Param : VectorBlends)
				FinalDMI->SetVectorParameterValue(Param.Name, Param.To);

			ActiveDMI = FinalDMI;
		}
	}

	OnCompleted.Broadcast();
	SetReadyToDestroy();
}

