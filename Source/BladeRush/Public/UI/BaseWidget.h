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
UCLASS()
class BLADERUSH_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
protected:
	virtual void ShowWidget();

	bool isWidgetVisible = true;

	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* Fade;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess),Category="Input")
	UInputAction* ShowBarAction;
};
