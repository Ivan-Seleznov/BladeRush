// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/PlayerHUDWidget.h"

#include "UI/PlayerHUD/StatusBarWidget.h"

void UPlayerHUDWidget::HidePlayerHUD()
{
	if (StatusBar && AimPoint)
	{
		StatusBar->SetVisibility(ESlateVisibility::Collapsed);
		AimPoint->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerHUDWidget::ShowPlayerHUD()
{
	if (StatusBar && AimPoint)
	{
		StatusBar->SetVisibility(ESlateVisibility::Visible);
		AimPoint->SetVisibility(ESlateVisibility::Visible);
	}
}