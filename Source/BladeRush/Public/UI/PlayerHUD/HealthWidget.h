// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "HealthWidget.generated.h"

class ABaseCharacter;
class UCanvasPanel;
class UOverlay;
class UProgressBar;

struct FOnAttributeChangeData;
UCLASS()
class BLADERUSH_API UHealthWidget : public UBaseWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void OnPawnInitialize(APawn* NewPawn) override;

	virtual void NativeDestruct() override;

	void HandleHealthPointsChanged(const FOnAttributeChangeData& ChangeData);
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(meta = (BindWidgetOptional))
	UOverlay* HealthBarOverlay;

	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* HealthPointsBar;
	
private:
	void SetFromHealthPointsAttribute();
	void BindHealthPointsChangeDelegate();
};
