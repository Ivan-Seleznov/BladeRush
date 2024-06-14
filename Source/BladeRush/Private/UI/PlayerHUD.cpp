// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerHealthComponent.h"
#include "UI/DeathScreenWidget.h"
#include "UI/PlayerHUDWidget.h"

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
		
		PlayerHud->SetVisibility(ESlateVisibility::Visible);
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->bShowMouseCursor = false;
	}
	
	if (HudState == EHUDState::OnlySpectating)
	{
		//PlayerHud->SetVisibility(ESlateVisibility::Collapsed);
		DeathWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (HudState == EHUDState::CanRespawnSpectating)
	{
		DeathWidget->ActivateRespawnButton();
		
		Controller->SetInputMode(FInputModeGameAndUI());
		Controller->bShowMouseCursor = true;
	}
	
	CurrentState = HudState;
}
