// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMods/BladeRushPlayerState.h"

#include "Net/UnrealNetwork.h"

void ABladeRushPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABladeRushPlayerState,KillCount)
	DOREPLIFETIME(ABladeRushPlayerState,DeathCount)

}

void ABladeRushPlayerState::AddKill()
{
	++KillCount;
}

void ABladeRushPlayerState::AddDeath()
{
	++DeathCount;
}
