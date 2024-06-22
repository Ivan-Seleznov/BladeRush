// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/LoadoutMenuWidget.h"

#include "Components/ComboBoxString.h"
#include "Data/AvailableLoadoutDataAsset.h"
#include "Loadout/LoadoutAbilitiesDefinition.h"
#include "Loadout/LoadoutComponent.h"
#include "Loadout/LoadoutItemQuickBarDef.h"
#include "Loadout/LoadoutTypes.h"

void ULoadoutMenuWidget::ApplyLoadout()
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
	
	if (const TSubclassOf<ULoadoutAbilitiesDefinition>* AbilitiesDef = AvailableLoadout->FindAbilitiesDefByName(FName(AbilitiesComboBox->GetSelectedOption())))
	{
		LoadoutDefinitions.AbilityDefinitions.Add(*AbilitiesDef);
	}
	
	LoadoutComponent->TrySetCurrentLoadout(LoadoutDefinitions);
}

void ULoadoutMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TryLoadLoadoutsToWidget();
}

void ULoadoutMenuWidget::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPawnChanged(OldPawn, NewPawn);

	TryLoadLoadoutsToWidget();
}

void ULoadoutMenuWidget::LoadLoadouts(ULoadoutComponent* LoadoutComponent)
{
	UAvailableLoadoutDataAsset* AvailableLoadout = LoadoutComponent->GetAvailableLoadoutDataAsset();
	if (!AvailableLoadout)
	{
		return;
	}

	if (!PrimaryWeaponComboBox || !SecondaryWeaponComboBox || !AbilitiesComboBox)
	{
		return;
	}
	
	PrimaryWeaponComboBox->ClearOptions();
	SecondaryWeaponComboBox->ClearOptions();
	AbilitiesComboBox->ClearOptions();
	
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

	for (const TSubclassOf<ULoadoutAbilitiesDefinition> AbilityDef : LoadoutDefinitions.AbilityDefinitions)
	{
		ULoadoutAbilitiesDefinition* LoadoutAbilitiesDef = AbilityDef.GetDefaultObject();

		AbilitiesComboBox->AddOption(LoadoutAbilitiesDef->GetDisplayName().ToString());
	}
}

void ULoadoutMenuWidget::LoadCurrentLoadout(ULoadoutComponent* LoadoutComponent)
{
	const FCharacterLoadout& CurrentLoadout = LoadoutComponent->GetCurrentLoadout();

	FLoadoutQuickBarItem FirstPrimaryItem = FindWeaponBySlotInCurrentLoadout(CurrentLoadout,0);
	if (FirstPrimaryItem.LoadoutDefinition)
	{
		PrimaryWeaponComboBox->SetSelectedOption(FirstPrimaryItem.LoadoutDefinition.GetDefaultObject()->GetDisplayName().ToString());
	}
		
	FLoadoutQuickBarItem FirstSecondaryItem = FindWeaponBySlotInCurrentLoadout(CurrentLoadout,1);
	if (FirstSecondaryItem.LoadoutDefinition)
	{
		SecondaryWeaponComboBox->SetSelectedOption(FirstSecondaryItem.LoadoutDefinition.GetDefaultObject()->GetDisplayName().ToString());
	}
	
	if (CurrentLoadout.AbilitySetsToGrant.IsValidIndex(0))
	{
		FLoadoutAbilities LoadoutAbilities = CurrentLoadout.AbilitySetsToGrant[0];
		AbilitiesComboBox->SetSelectedOption(LoadoutAbilities.LoadoutDefinition.GetDefaultObject()->GetDisplayName().ToString());
	}
}

void ULoadoutMenuWidget::TryLoadLoadoutsToWidget()
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

	LoadLoadouts(LoadoutComponent);
	LoadCurrentLoadout(LoadoutComponent);
}

FLoadoutQuickBarItem ULoadoutMenuWidget::FindWeaponBySlotInCurrentLoadout(const FCharacterLoadout& CurrentLoadout,
                                                                          int32 SlotIndex) const
{
	for (const FLoadoutQuickBarItem& ItemsToQuickBar : CurrentLoadout.ItemsToQuickBar)
	{
		ULoadoutItemQuickBarDef* ItemDef = Cast<ULoadoutItemQuickBarDef>(ItemsToQuickBar.LoadoutDefinition.GetDefaultObject());
		if (ItemDef && ItemDef->GetSlotIndex() == SlotIndex)
		{
			return ItemsToQuickBar;
		}
	}

	return FLoadoutQuickBarItem();
}
