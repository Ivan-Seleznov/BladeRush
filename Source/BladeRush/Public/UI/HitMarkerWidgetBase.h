// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "HitMarkerWidgetBase.generated.h"

/**
 * Base hit marker widget
 */
UCLASS()
class BLADERUSH_API UHitMarkerWidgetBase : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void AddHitMarker();

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	virtual void OnPawnInitialize(APawn* NewPawn) override;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	UWidgetAnimation* HitMarkerFadeOutAnimation;
};
