// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/BladeRushButton.h"

#include "Components/SizeBox.h"
#include "UI/BaseButton.h"

void UBladeRushButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!ButtonSizeBox || !ActualButton)
	{
		return;
	}
	ButtonSizeBox->SetMinDesiredWidth(ButtonSettings.ButtonSize.X);
	ButtonSizeBox->SetMinDesiredHeight(ButtonSettings.ButtonSize.Y);
	ActualButton->SetStyle(ButtonSettings.Style);
}
