// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UBladeRushGameInstance;
class UButton;
/**
 * 
 */
UCLASS()
class BLADERUSH_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartSoloGameButtonClicked();
	
	UFUNCTION()
	void OnHostGameButtonClicked();
	
	UFUNCTION()
	void OnJoinGameButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* StartSoloGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	
	UPROPERTY() UBladeRushGameInstance* BladeRushGameInstance;
};
