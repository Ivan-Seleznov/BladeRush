// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "GameFramework/GameState.h"
#include "BladeRushGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDied, const FDeadPlayerInfo&, DeadPlayerInfo);

UCLASS()
class BLADERUSH_API ABladeRushGameState : public AGameState
{
	GENERATED_BODY()

public:
	void NotifyPlayerDeath(const FDeadPlayerInfo& DeadPlayerInfo);

	mutable FOnPlayerDied OnPlayerDied;

private:
	UFUNCTION(NetMulticast, Unreliable)
	void OnPlayerDied_Multicast(const FDeadPlayerInfo& DeadPlayerInfo);
};
