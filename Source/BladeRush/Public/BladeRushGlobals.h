// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Slide		UMETA(DisplayName = "Slide"),
	CMOVE_WallRun	UMETA(DisplayName="WallRun"),
	CMOVE_Grappling UMETA(DisplayName="Grappling"),
	CMOVE_Max		UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMovementAction : uint8
{
	None,
	Mantle,
    Dash
};
