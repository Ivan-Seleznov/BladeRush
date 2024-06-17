// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "Blueprint/UserWidget.h"
#include "DeathInformationWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class BLADERUSH_API UDeathInformationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetData(const FText& KillerName, const FSlateBrush& InWeaponIcon);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> KillerNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> WeaponIcon;
};