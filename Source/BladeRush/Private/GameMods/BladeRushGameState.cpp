// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMods/BladeRushGameState.h"

#include "UI/PlayerHUD/KillfeedPanelWidget.h"

void ABladeRushGameState::NotifyPlayerDeath(const FDeadPlayerInfo& DeadPlayerInfo)
{
	if (HasAuthority())
	{
		OnPlayerDied_Multicast(DeadPlayerInfo);
	}
}

void ABladeRushGameState::OnPlayerDied_Multicast_Implementation(const FDeadPlayerInfo& DeadPlayerInfo)
{
	OnPlayerDied.Broadcast(DeadPlayerInfo);
}
