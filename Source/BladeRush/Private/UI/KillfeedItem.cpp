// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillfeedItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
// TODO: REFACTOR THIS 
void UKillfeedItem::Init(const FText& InPlayerWhoKilledName, UTexture2D* InWeaponIcon, const FText& KilledPlayerName)
{
	if (InPlayerWhoKilledName.ToString() == GetOwningPlayerState()->GetPlayerName())
	{
		KillfeedBackground->SetBrushColor(KillColor);
	}
	PlayerWhoKilledNameTextBlock->SetText(InPlayerWhoKilledName);
	WeaponIcon->SetBrushFromTexture(InWeaponIcon);
	KilledPlayerNameTextBlock->SetText(KilledPlayerName);
}