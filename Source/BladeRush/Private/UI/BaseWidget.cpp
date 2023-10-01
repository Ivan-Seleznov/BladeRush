// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidget.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Characters/BladeRushPlayerController.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameMods/BladeRushGameMode.h"
#include "Kismet/GameplayStatics.h"

void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerCharacter->GetController()->InputComponent))
	{
		EnhancedInputComponent->BindAction(ShowBarAction, ETriggerEvent::Started, this, &UBaseWidget::ShowWidget);
	}

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this,&ThisClass::OnPawnChanged);
}

void UBaseWidget::ShowWidget()
{
	if (!isWidgetVisible)
	{
		SetRenderOpacity(1);
		PlayAnimation(Fade);
		isWidgetVisible = true;
	}
	else
	{
		SetRenderOpacity(0);
		PlayAnimationReverse(Fade);
		isWidgetVisible = false;
	}
}

void UBaseWidget::OnPawnInitialize()
{
	
}

void UBaseWidget::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	ABladeRushPlayerController* PlayerController = Cast<ABladeRushPlayerController>(GetOwningPlayer());
	if (!PlayerController) return;

	if (PlayerController->IsSpectating()) return;

	OnPawnInitialize();
}

