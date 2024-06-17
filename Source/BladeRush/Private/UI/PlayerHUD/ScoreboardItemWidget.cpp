// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/ScoreboardItemWidget.h"

#include "Components/TextBlock.h"

void UScoreboardItemWidget::Init(const FText& PlayerName, const FText& KillCount, const FText& DeathCount,
                                 const FText& PingCount)
{
	if (!PlayerNameTextBlock
		|| !KillsPlayerStatTextBlock
		|| !DeathsPlayerStatTextBlock
		|| !PlayerPingTextBlock)
	{
		return;
	}

	PlayerNameTextBlock->SetText(PlayerName);
	KillsPlayerStatTextBlock->SetText(KillCount);
	DeathsPlayerStatTextBlock->SetText(DeathCount);
	PlayerPingTextBlock->SetText(PingCount);
}