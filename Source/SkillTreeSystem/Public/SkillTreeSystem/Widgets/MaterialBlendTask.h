// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MaterialBlendTask.generated.h"

class UImage;
class UCurveFloat;

USTRUCT()
struct FScalarBlendParam
{
	GENERATED_BODY()
	
	FName Name = NAME_None;
	float From = 0.0f;
	float To = 0.0f;
};

USTRUCT()
struct FVectorBlendParam
{
	GENERATED_BODY()
	
	FName Name = NAME_None;
	FLinearColor From = FLinearColor::White;
	FLinearColor To = FLinearColor::White;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMaterialBlendDelegate);

/**
 * UMaterialBlendTask
 *
 * Async Blueprint node that blends scalar and/or vector material parameters
 * on a UImage over time, optionally shaped by a UCurveFloat.
 *
 * The node exposes three output pins:
 *   - OnUpdated  fires every tick while blending.
 *   - OnComplete fires once when the blend finishes.
 *   - OnCancelled fires if a new BlendToMaterial call supersedes this one
 *               before it finishes (the task self-destructs cleanly).
 *
 * Typical Blueprint usage:
 *   BlendToMaterial(MyImage, NewMat, ScalarNames, VectorNames, 1.5, MyCurve)
 *       -> OnComplete: do next thing
 */
UCLASS()
class SKILLTREESYSTEM_API UMaterialBlendTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FMaterialBlendDelegate OnUpdated;

	UPROPERTY(BlueprintAssignable)
	FMaterialBlendDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FMaterialBlendDelegate OnCancelled;

	UFUNCTION(BlueprintCallable, Category = "Material|Blend",
		meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject"))
	static UMaterialBlendTask* BlendToMaterial(
		UObject* WorldContextObject,
		UImage* TargetImage,
		UMaterialInterface* NewMaterial,
		TSet<FName> ScalarParams,
		TSet<FName> VectorParams,
		float BlendDuration = 1.0f,
		UCurveFloat* BlendCurve = nullptr
	);

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Material|Blend")
	void Cancel();

private:

	bool Tick(float DeltaTime);

	FTSTicker::FDelegateHandle TickHandle;

	void ApplyBlendedValues(float Alpha);
	void FinaliseBlend();

	UPROPERTY()
	TObjectPtr<UImage> TargetImage;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> NewMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ActiveDMI;

	UPROPERTY()
	TObjectPtr<UCurveFloat> BlendCurve;

	TArray<FScalarBlendParam> ScalarBlends;
	TArray<FVectorBlendParam> VectorBlends;

	float BlendDuration = 0.0f;
	float BlendElapsed = 0.0f;
};


