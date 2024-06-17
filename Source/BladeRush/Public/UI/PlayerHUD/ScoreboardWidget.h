// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardWidget.generated.h"

class UScoreboardItemWidget;
class UVerticalBox;

UCLASS()
class BLADERUSH_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateScoreboard();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> PlayerStatContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScoreboardItemWidget> ScoreboardItemClass;
};