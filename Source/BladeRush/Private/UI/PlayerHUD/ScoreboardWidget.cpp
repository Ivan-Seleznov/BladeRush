// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/ScoreboardWidget.h"

#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "GameMods/BladeRushPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PlayerHUD/ScoreboardItemWidget.h"

void UScoreboardWidget::UpdateScoreboard()
{
	if (!PlayerStatContainer)
	{
		return;
	}
	PlayerStatContainer->ClearChildren();

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
	if (!GameState)
	{
		return;
	}
	for (TObjectPtr<APlayerState> PlayerState : GameState->PlayerArray)
	{
		if (ABladeRushPlayerState* Player = Cast<ABladeRushPlayerState>(PlayerState))
		{
			if (UScoreboardItemWidget* ScoreboardWidget = CreateWidget<UScoreboardItemWidget>(this, ScoreboardItemClass))
			{
				FText PingCount = FText::Format(NSLOCTEXT("ScoreboardWidget","Ping","{0}ms"), Player->GetCompressedPing());
				
				ScoreboardWidget->Init(FText::FromString(Player->GetPlayerName()),
					FText::AsNumber(Player->GetKillCount()), FText::AsNumber(Player->GetDeathCount()), PingCount);

				PlayerStatContainer->AddChild(ScoreboardWidget);
			}
		}
	}
}