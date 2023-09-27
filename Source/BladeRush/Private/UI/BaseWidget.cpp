// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidget.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Characters/Player/PlayerCharacter.h"

void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerCharacter->GetController()->InputComponent))
	{
		EnhancedInputComponent->BindAction(ShowBarAction, ETriggerEvent::Started, this, &UBaseWidget::ShowWidget);
	}
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

// void UBaseWidget::PlayAnimation(bool bReverse)
// {
// 	
// }
