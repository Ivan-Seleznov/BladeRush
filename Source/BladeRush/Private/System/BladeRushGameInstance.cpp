// Fill out your copyright notice in the Description page of Project Settings.


#include "System/BladeRushGameInstance.h"
#include "UI/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UBladeRushGameInstance::StartSoloGame(FName LevelName)
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    PlayerController->SetInputMode(FInputModeGameOnly());
    PlayerController->SetShowMouseCursor(false);

    
    UGameplayStatics::OpenLevel(this, LevelName,true);
}

void UBladeRushGameInstance::ShowMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!MainMenuWidget)
    {
        MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(PlayerController,MainMenuWidgetClas));
    }

    MainMenuWidget->AddToViewport();
    
    PlayerController->SetInputMode(FInputModeUIOnly());
    PlayerController->SetShowMouseCursor(true);
}
