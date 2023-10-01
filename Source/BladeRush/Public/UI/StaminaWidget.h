// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "StaminaWidget.generated.h"

/**
 * 
 */
class ABaseCharacter;
class UCanvasPanel;
class UOverlay;
class UProgressBar;

struct FOnAttributeChangeData;

UCLASS()
class BLADERUSH_API UStaminaWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(meta = (BindWidgetOptional))
	UOverlay* StaminaBarOverlay;

	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* StaminaPointsBar;
	
	virtual void NativeConstruct() override;
	
	void HandleStaminaPointsChanged(const FOnAttributeChangeData& ChangeData);

	virtual void OnPawnInitialize() override;
private:
	float LastHPUpdateTime = 0.0f;

	FTimerHandle BarShowTimerHandle;

	FTimerHandle PeriodBarShowTimerHandle;

	void BindStaminaChangedDelegate();
	void SetFromStaminaAttribute();
};