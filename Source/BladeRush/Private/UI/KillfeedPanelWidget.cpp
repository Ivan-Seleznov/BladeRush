// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillfeedPanelWidget.h"

#include "Components/VerticalBox.h"
#include "UI/KillfeedItem.h"
#include "Weapons/BaseWeaponActor.h"

void UKillfeedPanelWidget::PopulateKillfeedContainer(const FDeadPlayerInfo& DeadPlayerInfo)
{
	UKillfeedItem* KillfeedItem = CreateWidget<UKillfeedItem>(this, KillfeedItemClass);
	KillfeedItem->Init(FText::FromName(DeadPlayerInfo.KillerName),
		DeadPlayerInfo.Causer->GetCauserIcon(), FText::FromName(DeadPlayerInfo.NickName));
	KillfeedContainer->AddChild(KillfeedItem);
}
