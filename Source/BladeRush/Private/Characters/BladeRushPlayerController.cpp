// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BladeRushPlayerController.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/PlayerState.h"
#include "GameMods/BladeRushGameMode.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PlayerHUD.h"


void ABladeRushPlayerController::TrySetPlayerPlay_Server_Implementation()
{
	TrySetPlayerPlay();
}

ABladeRushPlayerController::ABladeRushPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(FName("InventoryManagerComponent"));
}

bool ABladeRushPlayerController::TrySetPlayerPlay()
{
	if (!HasAuthority()) return false;

	if (GetWorld()->GetTimerManager().TimerExists(RespawnTimerHandle)) return false;

	
	ABladeRushGameMode* GameMode = Cast<ABladeRushGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode) return false;
	
	GameMode->RespawnCharacterFromSpectator(this);
	
	PlayerState->SetIsSpectator(false);
	ChangeState(NAME_Playing);

	bPlayerIsWaiting = false;
	
	ClientGotoState(NAME_Playing);

	HUDStateChanged_Client(EHUDState::Playing);
	
	return true;
}

void ABladeRushPlayerController::SetPlayerSpectate()
{
	if (!HasAuthority()) return;

	DeadCharacter = Cast<ABaseCharacter>(GetCharacter());
	
	PlayerState->SetIsSpectator(true);
	ChangeState(NAME_Spectating);

	bPlayerIsWaiting = true;

	ClientGotoState(NAME_Spectating);
	ViewAPlayer(1);

	const ABladeRushGameMode* GameMode = Cast<ABladeRushGameMode>(UGameplayStatics::GetGameMode(this));
	const float RespawnTimerTime = GameMode ? GameMode->GetRespawnTimerTime() : 0.f;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::OnRespawnTimerEnd,RespawnTimerTime,false);

	HUDStateChanged_Client(EHUDState::OnlySpectating);

	
}

void ABladeRushPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(P))
	{
		BaseCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(BaseCharacter,BaseCharacter);
	}
}

void ABladeRushPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);

	if (const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetCharacter()))
	{
		if(UPlayerAbilitySystemComponent* AbilitySystemComponent = BaseCharacter->GetCharacterAbilitySystemComponent())
		{
			AbilitySystemComponent->ProcessAbilityInput(DeltaTime,bGamePaused);
		}
	}
}

void ABladeRushPlayerController::OnRespawnTimerEnd()
{
	if (!HasAuthority()) return;
	
	HUDStateChanged_Client(EHUDState::CanRespawnSpectating);
	
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

	if (DeadCharacter)
	{
		DeadCharacter->OnDeathFinished();
	}
}

void ABladeRushPlayerController::HUDStateChanged_Client_Implementation(EHUDState NewState)
{
	if (APlayerHUD* PlayerHUD = GetHUD<APlayerHUD>())
	{
		PlayerHUD->OnPlayerHudStateChanged(NewState);
	}
}
