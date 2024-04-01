// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadoutSelectionWidget.h"
#include "BladeRushLogs.h"
#include "Components/Button.h"
#include "Data/WeaponTypesDataAsset.h"

void ULoadoutSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ApplyButton->OnClicked.AddDynamic(this,&ThisClass::OnApplyButtonClicked);
}

void ULoadoutSelectionWidget::OnApplyButtonClicked()
{
}

void ULoadoutSelectionWidget::LoadWeapons()
{
}

void ULoadoutSelectionWidget::LoadWeaponsOfAType(EWeaponType WeaponType, UComboBoxString* ComboBox)
{
}

void ULoadoutSelectionWidget::LoadEquipment()
{
}

void ULoadoutSelectionWidget::LoadAbilities()
{
}
