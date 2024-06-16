// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/KillfeedItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UKillfeedItem::Init(const FText& KillerName, const FSlateBrush& InWeaponIcon, const FText& DeadPlayerName)
{
	if (KillerName.ToString() == GetOwningPlayerState()->GetPlayerName())
	{
		Background->SetBrushColor(KillColor);
	}
	PlayerWhoKilledNameTextBlock->SetText(KillerName);
	WeaponIcon->SetBrush(InWeaponIcon);
	KilledPlayerNameTextBlock->SetText(DeadPlayerName);
}