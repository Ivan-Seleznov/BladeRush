// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMods/BladeRushGameState.h"

#include "Kismet/GameplayStatics.h"
#include "UI/KillfeedPanelWidget.h"
#include "UI/PlayerHUD.h"

void ABladeRushGameState::NotifyPlayerDeath(const FDeadPlayerInfo& DeadPlayerInfo)
{
	if (HasAuthority())
	{
		SpawnKillfeed_Multicast(DeadPlayerInfo);
	}
}

void ABladeRushGameState::SpawnKillfeed_Multicast_Implementation(const FDeadPlayerInfo& DeadPlayerInfo)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController->IsLocalController())
	{
		APlayerHUD* PlayerHUD = PlayerController->GetHUD<APlayerHUD>();

		PlayerHUD->GetKillfeedPanelWidget()->PopulateKillfeedContainer(DeadPlayerInfo);
	}
}
