// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BladeRushSpectatorPawn.h"

#include "EnhancedInputComponent.h"

void ABladeRushSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	/*EnhancedInput->BindAction(ViewNextInputAction,ETriggerEvent::Triggered,this,&ThisClass::ViewNextPlayer);
	EnhancedInput->BindAction(ViewPrevInputAction,ETriggerEvent::Triggered,this,&ThisClass::ViewPrevPlayer);
	EnhancedInput->BindAction(ViewSelfInputAction,ETriggerEvent::Triggered,this,&ThisClass::ViewSelf);	*/
}

void ABladeRushSpectatorPawn::ViewNextPlayer()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->ServerViewNextPlayer();
	}
}

void ABladeRushSpectatorPawn::ViewPrevPlayer() 
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->ServerViewPrevPlayer();
	}
}

void ABladeRushSpectatorPawn::ViewSelf()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->ServerViewSelf();
	}
}