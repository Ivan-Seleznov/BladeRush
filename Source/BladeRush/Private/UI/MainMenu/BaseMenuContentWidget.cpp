// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/BaseMenuContentWidget.h"

#include "UI/BaseButton.h"
#include "UI/MainMenu/BladeRushButton.h"
#include "UI/MainMenu/MainMenuControllerInterface.h"

void UBaseMenuContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnButton)
	{
		ReturnButton->GetActualButton()->OnClicked.AddDynamic(this, &ThisClass::OnReturnClicked);
	}
}

void UBaseMenuContentWidget::Init(IMainMenuControllerInterface* InMainMenuController)
{
	MainMenuController = InMainMenuController;
}

void UBaseMenuContentWidget::OnReturnClicked()
{
	if (MainMenuController)
	{
		MainMenuController->ReturnToMainMenu();
	}
}
