// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/DamageCauserInterface.h"
#include "BladeRushGlobals.generated.h"

class ABaseWeaponActor;

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

USTRUCT(BlueprintType)
struct FDeadPlayerInfo
{
	GENERATED_BODY()

	FDeadPlayerInfo() = default;

	FDeadPlayerInfo(const FString& NickName, const FString& KillerName, ABaseWeaponActor* Causer)
		: NickName(NickName),
		  KillerName(KillerName),
		  Causer(Causer)
	{
	}
	UPROPERTY()
	FName NickName;
	UPROPERTY()
	FName KillerName;
	UPROPERTY()
	ABaseWeaponActor* Causer;
};