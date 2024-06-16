// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UKillfeedPanelWidget;

UCLASS()
class BLADERUSH_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UKillfeedPanelWidget* GetKillfeedPanel() const { return KillfeedPanel; }
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UKillfeedPanelWidget> KillfeedPanel;
};
