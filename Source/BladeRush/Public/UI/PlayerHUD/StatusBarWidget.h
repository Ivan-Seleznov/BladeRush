// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attributes/AttributeBase.h"
#include "UI/BaseWidget.h"
#include "StatusBarWidget.generated.h"

class UQuickBarComponent;
class UTextBlock;
class UProgressBar;

UCLASS()
class BLADERUSH_API UStatusBarWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void OnPawnInitialize(APawn* NewPawn) override;

	void BindHealthPointsChangeDelegate();
	void UpdateHealthDisplay();

	void RefreshAmmoCounter();

	void HandleHealthPointsChanged(const FOnAttributeChangeData& ChangeData);
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HealthCountTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AmmoCountTextBlock;

	UPROPERTY()
	TObjectPtr<UQuickBarComponent> QuickBarComponentCached;

	float CurrentAmmoInClip = 0;
	float CurrentMaxAmmoInClip = 0;
};
