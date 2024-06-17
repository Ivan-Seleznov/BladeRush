// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuWidget.h"

#include "BladeRushGameInstance.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/BaseButton.h"
#include "UI/MainMenu/BaseMenuContentWidget.h"
#include "UI/MainMenu/BladeRushButton.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!FindGameContent || !FindGameButton
		|| !CreateGameButton || !CreateGameContent
		|| !ExitButton)
	{
		return;
	}
	FindGameContent->Init(this);
	CreateGameContent->Init(this);
	OptionsContent->Init(this);
	
	FindGameButton->GetActualButton()->OnClicked.AddDynamic(this, &ThisClass::OnFindGameClicked);
	CreateGameButton->GetActualButton()->OnClicked.AddDynamic(this, &ThisClass::OnCreateGameClicked);
	OptionsButton->GetActualButton()->OnClicked.AddDynamic(this, &ThisClass::OnOptionsClicked);
	ExitButton->GetActualButton()->OnClicked.AddDynamic(this, &ThisClass::OnExitClicked);
	NickNameInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnNickNameComitted);
}

void UMainMenuWidget::OnFindGameClicked()
{
	if (ContentSwitcher && FindGameContent)
	{
		ContentSwitcher->SetActiveWidget(FindGameContent);
	}
}

void UMainMenuWidget::OnCreateGameClicked()
{
	if (ContentSwitcher && CreateGameContent)
	{
		ContentSwitcher->SetActiveWidget(CreateGameContent);
	}
}

void UMainMenuWidget::OnOptionsClicked()
{
	if (ContentSwitcher && OptionsContent)
	{
		ContentSwitcher->SetActiveWidget(OptionsContent);
	}
}

void UMainMenuWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::OnNickNameComitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!Text.IsEmpty())
	{
		GetGameInstance<UBladeRushGameInstance>()->SetNickName(Text.ToString());
	}
}

void UMainMenuWidget::ReturnToMainMenu()
{
	ContentSwitcher->SetActiveWidgetIndex(0);
}
