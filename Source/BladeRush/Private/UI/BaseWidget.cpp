// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidget.h"
#include "Characters/BladeRushPlayerController.h"

void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	/*APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerCharacter->GetController()->InputComponent))
	{
		EnhancedInputComponent->BindAction(ShowBarAction, ETriggerEvent::Started, this, &UBaseWidget::ShowWidget);
	}*/
	checkf(GetOwningPlayer(),TEXT("No widget owning player"));
	
	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this,&ThisClass::OnPawnChanged);
}

void UBaseWidget::OnPawnInitialize(APawn* NewPawn)
{
	K2_OnPawnInitialize(NewPawn);
}

void UBaseWidget::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	ABladeRushPlayerController* PlayerController = Cast<ABladeRushPlayerController>(GetOwningPlayer());
	if (!PlayerController) return;

	if (PlayerController->IsSpectating()) return;

	OnPawnInitialize(NewPawn);
}