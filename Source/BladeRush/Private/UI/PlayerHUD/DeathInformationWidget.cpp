// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/DeathInformationWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDeathInformationWidget::SetData(const FText& KillerName, const FSlateBrush& InWeaponIcon)
{
	if (!KillerNameTextBlock || !WeaponIcon || !InWeaponIcon.IsSet() || KillerName.IsEmpty())
	{
		return;
	}

	KillerNameTextBlock->SetText(KillerName);
	WeaponIcon->SetBrush(InWeaponIcon);
}