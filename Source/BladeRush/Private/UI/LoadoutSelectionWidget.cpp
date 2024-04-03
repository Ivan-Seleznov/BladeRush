// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadoutSelectionWidget.h"
#include "BladeRushLogs.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Data/AvailableLoadoutDataAsset.h"
#include "Loadout/LoadoutAbilitiesDefinition.h"
#include "Loadout/LoadoutComponent.h"
#include "Loadout/LoadoutItemQuickBarDef.h"

void ULoadoutSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	ULoadoutComponent* LoadoutComponent = PC->FindComponentByClass<ULoadoutComponent>();
	if (!LoadoutComponent)
	{
		return;
	}

	LoadLoadouts(LoadoutComponent);
	LoadCurrentLoadout(LoadoutComponent);
	
	ApplyButton->OnClicked.AddDynamic(this,&ThisClass::OnApplyButtonClicked);
}

void ULoadoutSelectionWidget::OnApplyButtonClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	ULoadoutComponent* LoadoutComponent = PC->FindComponentByClass<ULoadoutComponent>();
	if (!LoadoutComponent)
	{
		return;
	}

	UAvailableLoadoutDataAsset* AvailableLoadout = LoadoutComponent->GetAvailableLoadoutDataAsset();
	if (!AvailableLoadout)
	{
		return;
	}

	
	FLoadoutDefinitions LoadoutDefinitions;
	if (const TSubclassOf<ULoadoutItemQuickBarDef>* PrimaryWeapon = AvailableLoadout->FindQuickBarDefByName(FName(PrimaryWeaponComboBox->GetSelectedOption())))
	{
		LoadoutDefinitions.QuickBarDefinitions.Add(*PrimaryWeapon);
	}
	if (const TSubclassOf<ULoadoutItemQuickBarDef>* SecondaryWeapon = AvailableLoadout->FindQuickBarDefByName(FName(SecondaryWeaponComboBox->GetSelectedOption())))
	{
		LoadoutDefinitions.QuickBarDefinitions.Add(*SecondaryWeapon);
	}

	if (const TSubclassOf<ULoadoutItemDefinition>* EquipmentItem = AvailableLoadout->FindEquipDefByName(FName(EquipmentComboBox->GetSelectedOption())))
	{
		LoadoutDefinitions.EquipmentDefinitions.Add(*EquipmentItem);
	}

	if (const TSubclassOf<ULoadoutAbilitiesDefinition>* AbilitiesDef = AvailableLoadout->FindAbilitiesDefByName(FName(AbilitySetComboBox->GetSelectedOption())))
	{
		LoadoutDefinitions.AbilityDefinitions.Add(*AbilitiesDef);
	}
	
	LoadoutComponent->TrySetCurrentLoadout(LoadoutDefinitions);
}

void ULoadoutSelectionWidget::LoadLoadouts(ULoadoutComponent* LoadoutComponent)
{
	UAvailableLoadoutDataAsset* AvailableLoadout = LoadoutComponent->GetAvailableLoadoutDataAsset();
	if (!AvailableLoadout)
	{
		return;
	}

	const FLoadoutDefinitions& LoadoutDefinitions = AvailableLoadout->GetLoadoutDefinitions();
	for (const TSubclassOf<ULoadoutItemQuickBarDef>	ItemQuickBarDef : LoadoutDefinitions.QuickBarDefinitions)
	{
		ULoadoutItemQuickBarDef* LoadoutItemQuickBarDef = ItemQuickBarDef.GetDefaultObject();

		if (LoadoutItemQuickBarDef->GetSlotIndex() == 0)
		{
			PrimaryWeaponComboBox->AddOption(LoadoutItemQuickBarDef->GetDisplayName().ToString());
		}
		else if(LoadoutItemQuickBarDef->GetSlotIndex() == 1)
		{
			SecondaryWeaponComboBox->AddOption(LoadoutItemQuickBarDef->GetDisplayName().ToString());
		}
	}

	for (const TSubclassOf<ULoadoutItemDefinition> EquipItemDef : LoadoutDefinitions.EquipmentDefinitions)
	{
		ULoadoutItemDefinition* LoadoutEquipDef = EquipItemDef.GetDefaultObject();

		EquipmentComboBox->AddOption(LoadoutEquipDef->GetDisplayName().ToString());
	}

	for (const TSubclassOf<ULoadoutAbilitiesDefinition> AbilityDef : LoadoutDefinitions.AbilityDefinitions)
	{
		ULoadoutAbilitiesDefinition* LoadoutAbilitiesDef = AbilityDef.GetDefaultObject();

		AbilitySetComboBox->AddOption(LoadoutAbilitiesDef->GetDisplayName().ToString());
	}
}

void ULoadoutSelectionWidget::LoadCurrentLoadout(ULoadoutComponent* LoadoutComponent)
{
	const FCharacterLoadout& CurrentLoadout = LoadoutComponent->GetCurrentLoadout();

	FLoadoutItem FirstPrimaryItem = FindWeaponBySlotInCurrentLoadout(CurrentLoadout,0);
	if (FirstPrimaryItem.LoadoutDefinition)
	{
		PrimaryWeaponComboBox->SetSelectedOption(FirstPrimaryItem.LoadoutDefinition.GetDefaultObject()->GetDisplayName().ToString());
	}
		
	FLoadoutItem FirstSecondaryItem = FindWeaponBySlotInCurrentLoadout(CurrentLoadout,1);
	if (FirstSecondaryItem.LoadoutDefinition)
	{
		SecondaryWeaponComboBox->SetSelectedOption(FirstSecondaryItem.LoadoutDefinition.GetDefaultObject()->GetDisplayName().ToString());
	}
	
	if (CurrentLoadout.ItemsToEquip.IsValidIndex(0))
	{
		FLoadoutItem LoadoutItem = CurrentLoadout.ItemsToEquip[0];
		EquipmentComboBox->SetSelectedOption(LoadoutItem.LoadoutDefinition.GetDefaultObject()->GetDisplayName().ToString());
	}
	if (CurrentLoadout.AbilitySetsToGrant.IsValidIndex(0))
	{
		FLoadoutAbilities LoadoutAbilities = CurrentLoadout.AbilitySetsToGrant[0];
		AbilitySetComboBox->SetSelectedOption(LoadoutAbilities.LoadoutDefinition.GetDefaultObject()->GetDisplayName().ToString());
	}
}

FLoadoutItem ULoadoutSelectionWidget::FindWeaponBySlotInCurrentLoadout(const FCharacterLoadout& CurrentLoadout,int32 SlotIndex) const
{
	for (const FLoadoutItem& ItemsToQuickBar : CurrentLoadout.ItemsToQuickBar)
	{
		ULoadoutItemQuickBarDef* ItemDef = Cast<ULoadoutItemQuickBarDef>(ItemsToQuickBar.LoadoutDefinition.GetDefaultObject());
		if (ItemDef && ItemDef->GetSlotIndex() == SlotIndex)
		{
			return ItemsToQuickBar;
		}
	}

	return FLoadoutItem();
}
