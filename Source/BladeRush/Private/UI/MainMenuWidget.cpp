// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "System/BladeRushGameInstance.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BladeRushGameInstance = Cast<UBladeRushGameInstance>(UGameplayStatics::GetGameInstance(this));

	StartSoloGameButton->OnClicked.AddDynamic(this,&ThisClass::OnStartSoloGameButtonClicked);
	HostGameButton->OnClicked.AddDynamic(this,&ThisClass::OnHostGameButtonClicked);
	JoinGameButton->OnClicked.AddDynamic(this,&ThisClass::OnJoinGameButtonClicked);
	ExitButton->OnClicked.AddDynamic(this,&ThisClass::OnExitButtonClicked);

}

void UMainMenuWidget::OnStartSoloGameButtonClicked()
{
	BladeRushGameInstance->StartSoloGame("TestMap");
}

void UMainMenuWidget::OnHostGameButtonClicked()
{
}

void UMainMenuWidget::OnJoinGameButtonClicked()
{
}

void UMainMenuWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this,GetOwningPlayer(),EQuitPreference::Quit,true);
}
