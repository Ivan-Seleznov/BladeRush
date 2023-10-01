// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeathScreenWidget.h"

#include "Characters/BladeRushPlayerController.h"
#include "Components/Button.h"

void UDeathScreenWidget::ActivateRespawnButton()
{
	RespawnButton->SetIsEnabled(true);
}

void UDeathScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RespawnButton->SetIsEnabled(false);

	RespawnButton->OnClicked.AddDynamic(this,&ThisClass::OnRespawnButtonClicked);
}

void UDeathScreenWidget::OnRespawnButtonClicked()
{
	ABladeRushPlayerController* BladeRushPlayerController = Cast<ABladeRushPlayerController>(GetOwningPlayer());
	checkf(BladeRushPlayerController,TEXT("no owning player controller, death screen widget"));

	BladeRushPlayerController->TrySetPlayerPlay_Server();
}
