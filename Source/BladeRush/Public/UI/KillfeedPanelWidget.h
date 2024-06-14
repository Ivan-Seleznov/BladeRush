﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "Blueprint/UserWidget.h"
#include "KillfeedPanelWidget.generated.h"

class UBorder;
class UKillfeedItem;
class UVerticalBox;

UCLASS()
class BLADERUSH_API UKillfeedPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PopulateKillfeedContainer(const FDeadPlayerInfo& DeadPlayerInfo);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> KillfeedContainer;
 
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UKillfeedItem> KillfeedItemClass;
};
