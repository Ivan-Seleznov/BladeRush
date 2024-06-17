// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UStatusBarWidget;
class UKillfeedPanelWidget;

UCLASS()
class BLADERUSH_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void HidePlayerHUD();
	void ShowPlayerHUD();
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatusBarWidget> StatusBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUserWidget> AimPoint;
};