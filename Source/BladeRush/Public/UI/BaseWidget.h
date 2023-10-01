// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS(Abstract)
class BLADERUSH_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
protected:
	virtual void OnPawnInitialize();
	
private:
	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);
};
