// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeSystem/Structures/SkillTreeCoreStructures.h"
#include "SkillTreeSystem/Structures/SkillTreeCanvasStructures.h"
#include "UObject/Interface.h"
#include "SkillTreeLayoutMaker.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USkillTreeLayoutMaker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SKILLTREESYSTEM_API ISkillTreeLayoutMaker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTreeLayoutMaker")
	void GetElementsPosition(TMap<FName, FSkillTreeNodePosition>& NodePositions, TMap<FSkillTreeLinkName, FSkillTreeLinkPosition>& LinkPositions);
	
};
