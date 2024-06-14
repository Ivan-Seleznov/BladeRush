// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "KillfeedItem.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class BLADERUSH_API UKillfeedItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(const FText& InPlayerWhoKilledName, UTexture2D* InWeaponIcon, const FText& KilledPlayerName);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerWhoKilledNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> WeaponIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> KillfeedBackground;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> KilledPlayerNameTextBlock;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor KillColor;
};
