// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/PlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerHealthComponent.h"
#include "UI/PlayerHUD/DeathScreenWidget.h"
#include "UI/PlayerHUD/DeathScreenWidget.h"
#include "UI/PlayerHUD/PlayerHUDWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Controller = GetOwningPlayerController();
	if (!Controller) return;

	if (!PlayerHudClass) return;

	PlayerHud = CreateWidget<UPlayerHUDWidget>(Controller,PlayerHudClass);
	PlayerHud->AddToViewport();

	if (!DeathWidgetClass) return;
	
	DeathWidget = CreateWidget<UDeathScreenWidget>(Controller,DeathWidgetClass);
	if (!DeathWidget)
	{
		return;
	}
	DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	DeathWidget->AddToViewport();
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>( Controller->GetCharacter());
	if (!BaseCharacter) return;

	UPlayerHealthComponent* PlayerHealthComponent = BaseCharacter->GetPlayerHealthComponent();
	if (!PlayerHealthComponent) return;

	CurrentState = EHUDState::Playing;
}

void APlayerHUD::OnPlayerHudStateChanged(EHUDState HudState)
{
	APlayerController* Controller = GetOwningPlayerController();
	if (!Controller) return;
	
	if (HudState == EHUDState::Playing)
	{
		PlayerHud->Construct();
		
		PlayerHud->ShowPlayerHUD();
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->bShowMouseCursor = false;
	}
	
	if (HudState == EHUDState::OnlySpectating)
	{
		PlayerHud->HidePlayerHUD();
		DeathWidget->SetVisibility(ESlateVisibility::Visible);
		
		Controller->bShowMouseCursor = true;
		Controller->SetInputMode(FInputModeGameAndUI());
	}
	
	if (HudState == EHUDState::CanRespawnSpectating)
	{
		DeathWidget->ActivateRespawn();
	}
	
	CurrentState = HudState;
}
