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

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	virtual void OnPawnInitialize(APawn* NewPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnPawnInitialize(APawn* NewPawn);
private:
	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);
};
