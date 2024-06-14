// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenuContentWidget.generated.h"

class IMainMenuControllerInterface;
class UBladeRushButton;

UCLASS()
class BLADERUSH_API UBaseMenuContentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(IMainMenuControllerInterface*  InMainMenuController);
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReturnClicked();

	UPROPERTY(meta=(BindWidget))
	UBladeRushButton* ReturnButton;

	IMainMenuControllerInterface* MainMenuController;
};
