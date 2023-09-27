// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "Blueprint/UserWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Controller = GetOwningPlayerController();
	if (!Controller) return;

	if (!PlayerHudClass) return;

	PlayerHud = CreateWidget(Controller,PlayerHudClass);
	PlayerHud->AddToViewport();
}
