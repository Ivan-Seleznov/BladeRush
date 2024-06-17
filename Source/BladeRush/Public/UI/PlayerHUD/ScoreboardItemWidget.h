// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardItemWidget.generated.h"

class UTextBlock;

UCLASS()
class BLADERUSH_API UScoreboardItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FText& PlayerName, const FText& KillCount, const FText& DeathCount, const FText& PingCount);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerNameTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> KillsPlayerStatTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DeathsPlayerStatTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerPingTextBlock;
};
