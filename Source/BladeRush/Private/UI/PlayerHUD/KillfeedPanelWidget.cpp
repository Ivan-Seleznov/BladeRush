// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/KillfeedPanelWidget.h"

#include "Components/VerticalBox.h"
#include "GameMods/BladeRushGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PlayerHUD/KillfeedItem.h"
#include "Weapons/BaseWeaponActor.h"

void UKillfeedPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ABladeRushGameState* BladeRushGameState = Cast<ABladeRushGameState>(UGameplayStatics::GetGameState(this)))
	{
		BladeRushGameState->OnPlayerDied.AddDynamic(this, &ThisClass::OnPlayerDied);
	}
}

void UKillfeedPanelWidget::OnPlayerDied(const FDeadPlayerInfo& DeadPlayerInfo)
{
	if (DeadPlayerInfo.KillerName.IsNone()
		|| DeadPlayerInfo.DeadPlayerName.IsNone()
		|| !DeadPlayerInfo.Causer
		|| !KillfeedContainer
		|| !KillfeedItemClass)
	{
		return;
	}
	IDamageCauserInterface* DamageCauser = Cast<IDamageCauserInterface>(DeadPlayerInfo.Causer);
	if (!DamageCauser)
	{
		return;
	}
	
	if (UKillfeedItem* KillfeedItem = CreateWidget<UKillfeedItem>(this, KillfeedItemClass))
	{
		KillfeedItem->Init(FText::FromName(DeadPlayerInfo.KillerName),
			DamageCauser->GetCauserIcon(), FText::FromName(DeadPlayerInfo.DeadPlayerName));
		KillfeedContainer->AddChild(KillfeedItem);
	}
}