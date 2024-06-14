// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuControllerInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UBaseMenuContentWidget;
class UWidgetSwitcher;
class UBladeRushButton;

UCLASS()
class BLADERUSH_API UMainMenuWidget : public UUserWidget, public IMainMenuControllerInterface
{
	GENERATED_BODY()
	
public:
	virtual void ReturnToMainMenu() override;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnFindGameClicked();
	
	UFUNCTION()
	void OnCreateGameClicked();

	UFUNCTION()
	void OnOptionsClicked();
	
	UFUNCTION()
	void OnExitClicked();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> ContentSwitcher;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseMenuContentWidget> FindGameContent;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseMenuContentWidget> CreateGameContent;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseMenuContentWidget> OptionsContent;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBladeRushButton> FindGameButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBladeRushButton> CreateGameButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBladeRushButton> OptionsButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBladeRushButton> ExitButton;
};
