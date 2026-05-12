// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeSystem/CanvasSource/SkillTreeCanvasSourceInterface.h"
#include "SkillTreeSystem/CanvasSource/LayoutMaker/SkillTreeLayoutMaker.h"
#include "SkillTreeSystem/Behavior/SkillTreeBehaviorInterface.h"
#include "SkillTreeSystem/Behavior/SkillTreeBehaviorDataAsset.h"
#include "SkillTreeDataAsset.generated.h"


USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeDataAssetLinkData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas|Widget|Link")
	TSubclassOf<USkillTreeLinkWidget> LinkWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas|Widget|Link")
	FSkillTreeLinkAppearance LinkAppearance;
	
};

UENUM(BlueprintType)
enum class ESkillTreeDataAssetLayoutMethod : uint8
{
	/** Let the default layout maker handle it */
	Default = 0			UMETA(DisplayName = "Default"),
	/** Absolute position */
	Absolute = 1		UMETA(DisplayName = "Absolute"),
	/** Relative position from parents */
	Relative = 2		UMETA(DisplayName = "Relative"),
	/** Delegate the layout method to the external asset */
	External = 3		UMETA(DisplayName = "External")
};

UENUM(BlueprintType)
enum class ESkillTreeDataAssetLayoutRelativeAnchorPoint : uint8
{
	// Which point on the parent the offset originates from
	Center		UMETA(DisplayName = "Center"),
	Top			UMETA(DisplayName = "Top"),
	Bottom		UMETA(DisplayName = "Bottom"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class ESkillTreeDataAssetLayoutRelativeAngleMode : uint8
{
	// Angle is explicit degrees (0 = right, 90 = down)
	Explicit		UMETA(DisplayName = "Explicit"),
	// Angle is evenly distributed among siblings sharing the same parent
	AutoSpread		UMETA(DisplayName = "Auto spread"),
	// Angle inherits the parent->grandparent direction (continues straight)
	Inherited		UMETA(DisplayName = "Inherited"),
};

UENUM(BlueprintType)
enum class ESkillTreeDataAssetLayoutSpreadDirection : uint8
{
	// Arc centered on a fixed downward axis (current behavior)
	Down            UMETA(DisplayName = "Down (90°)"),
	Up              UMETA(DisplayName = "Up (270°)"),
	Right           UMETA(DisplayName = "Right (0°)"),
	Left            UMETA(DisplayName = "Left (180°)"),
	// Arc centered on a custom explicit angle
	CustomAngle     UMETA(DisplayName = "Custom angle"),
	// Arc pushed away from a world-space center point
	// The center angle becomes the direction from CenterPoint → parent origin
	FromCenter      UMETA(DisplayName = "From center"),
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeDataAssetRelativeLayoutData
{
	GENERATED_BODY()
	
public:
	
	// Distance from parent(s) midpoint to this node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	float Distance = 120.0f;
	
	// When node has multiple parents, blend their positions by this weight (0=first parent only, 1=last parent only, 0.5=midpoint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MultiParentBlend = 0.5f;
	
	// How the angle to place this node is determined
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle")
	ESkillTreeDataAssetLayoutRelativeAngleMode AngleMode = ESkillTreeDataAssetLayoutRelativeAngleMode::AutoSpread;
	
	// Used when AngleMode == Explicit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle",
		meta = (EditConditionHides, EditCondition = "AngleMode == ESkillTreeDataAssetLayoutRelativeAngleMode::Explicit"))
	float ExplicitAngleDeg = 90.0f;
	
	// Angular spread limit for AutoSpread siblings (degrees total arc, centered on parent direction)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle",
		meta = (EditConditionHides, EditCondition = "AngleMode == ESkillTreeDataAssetLayoutRelativeAngleMode::AutoSpread",
				ClampMin = "1.0", ClampMax = "360.0"))
	float SpreadArcDeg = 90.0f;
	
	// Direction the AutoSpread arc is centered on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle",
		meta = (EditConditionHides,
				EditCondition = "AngleMode == ESkillTreeDataAssetLayoutRelativeAngleMode::AutoSpread"))
	ESkillTreeDataAssetLayoutSpreadDirection SpreadDirection =
		ESkillTreeDataAssetLayoutSpreadDirection::Down;
	
	// Used when SpreadDirection == CustomAngle (degrees, 0=right, 90=down)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle",
		meta = (EditConditionHides,
				EditCondition = "AngleMode == ESkillTreeDataAssetLayoutRelativeAngleMode::AutoSpread && SpreadDirection == ESkillTreeDataAssetLayoutSpreadDirection::CustomAngle"))
	float SpreadCustomAngleDeg = 90.0f;

	// Used when SpreadDirection == FromCenter
	// The arc center is the direction from this point to the parent's resolved position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle",
		meta = (EditConditionHides,
				EditCondition = "AngleMode == ESkillTreeDataAssetLayoutRelativeAngleMode::AutoSpread && SpreadDirection == ESkillTreeDataAssetLayoutSpreadDirection::FromCenter"))
	FVector2D SpreadCenterPoint = FVector2D::ZeroVector;

	// Extra offset applied to SpreadCenterPoint before computing the direction
	// Useful to shift the repulsion origin without changing the canvas reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle",
		meta = (EditConditionHides,
				EditCondition = "AngleMode == ESkillTreeDataAssetLayoutRelativeAngleMode::AutoSpread && SpreadDirection == ESkillTreeDataAssetLayoutSpreadDirection::FromCenter"))
	FVector2D SpreadCenterOffset = FVector2D::ZeroVector;
	
	// Offset applied after angle+distance, in local canvas space (fine-tuning)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
	FVector2D ManualOffset = FVector2D::ZeroVector;
	
	// Snap final position to a grid of this size (0 = disabled)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping", meta = (ClampMin = "0.0"))
	float GridSnap = 0.0f;
	
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeDataAssetLayoutData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTreeDataAssetLayoutMethod Method = ESkillTreeDataAssetLayoutMethod::Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "Method == ESkillTreeDataAssetLayoutMethod::Absolute"))
	FVector2D AbsolutePosition = FVector2D::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "Method == ESkillTreeDataAssetLayoutMethod::Relative"))
	FSkillTreeDataAssetRelativeLayoutData Relative;
};

USTRUCT(BlueprintType)
struct SKILLTREESYSTEM_API FSkillTreeDataAssetElement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	FSkillTreeNodeAppearance Appearance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	TSubclassOf<USkillTreeNodeWidget> NodeWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	TMap<FGameplayTag, FSkillTreeDataAssetLinkData> ParentLinks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	FSkillTreeDataAssetLayoutData Layout;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	TArray<FSkillTreeLevelRequirement> Levels;
	
};


/**
 * 
 */
UCLASS()
class SKILLTREESYSTEM_API USkillTreeDataAsset : public UDataAsset,
	public ISkillTreeCanvasSourceInterface,
	public ISkillTreeLayoutMaker,
	public ISkillTreeBehaviorInterface
{
	GENERATED_BODY()
	
public:
	
	/** Canvas Source */
	virtual void GetCanvasNodesIds_Implementation(TArray<FGameplayTag>& OutNodes) override;
	virtual void GetCanvasNodeData_Implementation(const FGameplayTag& NodeId, TSubclassOf<USkillTreeNodeWidget>& OutWidgetClass, FSkillTreeNodeAppearance& OutAppearance) override;
	virtual void GetCanvasLinkIds_Implementation(TArray<FSkillTreeLinkName>& OutLinks) override;
	virtual void GetCanvasLinkData_Implementation(const FSkillTreeLinkName& LinkName, TSubclassOf<USkillTreeLinkWidget>& OutWidgetClass, FSkillTreeLinkAppearance& OutAppearance) override;
	virtual void GetCanvasElementsPositions_Implementation(TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions) override;
	
	/** Behaviors */
	virtual void GetNodesIds_Implementation(TArray<FGameplayTag>& OutNodes) const override;
	virtual bool CanUpgradeNode_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerBase* State) const override;
	virtual void UpdateNodeState_Implementation(const FGameplayTag& NodeId, USkillTreeStateControllerEditable* State) override;
	virtual void GatherInterestsForNode_Implementation(const FGameplayTag& NodeId, FSkillTreeBehaviorInterest& Interests) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<FGameplayTag, FSkillTreeDataAssetElement> Nodes;
	
	
	/** Canvas Source */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Canvas|Widget")
	TSubclassOf<USkillTreeNodeWidget> DefaultNodeWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Canvas|Widget|Link")
	TSubclassOf<USkillTreeLinkWidget> DefaultLinkWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Canvas|Widget|Link")
	FSkillTreeLinkAppearance DefaultLinkAppearance;
	
	/** Layout */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Canvas|Layout")
	FSkillTreeDataAssetLayoutData DefaultLayout;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Canvas|Layout")
	TScriptInterface<ISkillTreeLayoutMaker> ExternalLayoutSource;
	
	
	/** Behavior */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Requirements", meta = (BaseStruct = "/Script/SkillTreeSystem.SkillTreeRequirementBase", ExcludeBaseStruct))
	FInstancedStruct GlobalRequirement;
	
	
private:
	
	void _PopulateLinksFromPositions(
		const TMap<FGameplayTag, FSkillTreeNodePosition>& NodePositions,
		TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& OutLinkPositions);
	
	void _ResolveRelativeNodes(
		const TArray<FGameplayTag>& RelativeNodes,
		TFunctionRef<const FSkillTreeDataAssetLayoutData&(const FSkillTreeDataAssetElement&)> GetEffectiveLayout,
		TMap<FGameplayTag, FSkillTreeNodePosition>& OutNodePositions);
	
};
