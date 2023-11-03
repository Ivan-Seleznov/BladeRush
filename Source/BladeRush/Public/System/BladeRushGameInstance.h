// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BladeRushGameInstance.generated.h"

class UMainMenuWidget;
/**
 * BladeRush game instance class
 */
UCLASS()
class BLADERUSH_API UBladeRushGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    void StartSoloGame(FName LevelName);

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();
protected:

	UPROPERTY(EditDefaultsOnly,Category="MainMenu")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClas;

	UPROPERTY(BlueprintReadOnly,Category="MainMenu")
	UMainMenuWidget* MainMenuWidget;
};
