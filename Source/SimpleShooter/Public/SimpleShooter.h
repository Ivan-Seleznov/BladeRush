// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	None								UMETA(DisplayName = "None"),
	
	Confirm								UMETA(DisplayName = "Confirm"),
	Cancel								UMETA(DisplayName = "Cancel"),
	
	//Sprint								UMETA(DisplayName = "Sprint"),
};