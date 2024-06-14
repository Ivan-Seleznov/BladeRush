// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "GameFramework/GameState.h"
#include "BladeRushGameState.generated.h"

UCLASS()
class BLADERUSH_API ABladeRushGameState : public AGameState
{
	GENERATED_BODY()

public:
	void NotifyPlayerDeath(const FDeadPlayerInfo& DeadPlayerInfo);

private:
	UFUNCTION(NetMulticast, Unreliable)
	void SpawnKillfeed_Multicast(const FDeadPlayerInfo& DeadPlayerInfo);
};
