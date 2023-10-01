// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathScreenWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class BLADERUSH_API UDeathScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ActivateRespawnButton();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRespawnButtonClicked();
	
	UPROPERTY(meta=(BindWidget))
	UButton* RespawnButton;
};
