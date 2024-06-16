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
	Mantle
};

USTRUCT(BlueprintType)
struct FDeadPlayerInfo
{
	GENERATED_BODY()

	FDeadPlayerInfo() = default;

	FDeadPlayerInfo(const FString& NickName, const FString& KillerName, AActor* Causer)
		: DeadPlayerName(NickName),
		  KillerName(KillerName),
		  Causer(Causer)
	{
	}
	
	UPROPERTY()
	FName DeadPlayerName;
	UPROPERTY()
	FName KillerName;
	UPROPERTY()
	AActor* Causer;
};