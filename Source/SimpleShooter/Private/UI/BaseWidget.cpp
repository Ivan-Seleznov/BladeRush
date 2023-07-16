// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidget.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Characters/Player/PlayerCharacter.h"

void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	/*if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 1);
	}*/
	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerCharacter->GetController()->InputComponent))
	{
		EnhancedInputComponent->BindAction(ShowBarAction, ETriggerEvent::Started, this, &UBaseWidget::ShowWidget);
	}
	isWidgetVisible = false;
	SetRenderOpacity(0);
}

void UBaseWidget::ShowWidget()
{
	if (!isWidgetVisible)
	{
		UE_LOG(LogTemp,Display,TEXT("ShowWidget"));
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

// void UBaseWidget::PlayAnimation(bool bReverse)
// {
// 	
// }
