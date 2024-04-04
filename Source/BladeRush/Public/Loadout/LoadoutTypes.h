
#pragma once
#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "LoadoutTypes.generated.h"

class UInventoryItemDefinition;
class ULoadoutItemQuickBarDef;
class UEquipmentDefinition;
class ULoadoutEquipmentDefinition;
class UShooterAbilitySet;
class ULoadoutAbilitiesDefinition;

USTRUCT(BlueprintType)
struct FLoadoutQuickBarItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UInventoryItemDefinition> ItemDef;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ULoadoutItemQuickBarDef> LoadoutDefinition;
};

USTRUCT(BlueprintType)
struct FLoadoutEquipment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UEquipmentDefinition> EquipmentDef;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ULoadoutEquipmentDefinition> LoadoutDefinition;
};

USTRUCT(BlueprintType)
struct FLoadoutAbilities
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UShooterAbilitySet> AbilitySet;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ULoadoutAbilitiesDefinition> LoadoutDefinition;
};

USTRUCT(BlueprintType)
struct FCharacterLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FLoadoutQuickBarItem> ItemsToQuickBar;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FLoadoutEquipment> ItemsToEquip;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FLoadoutAbilities> AbilitySetsToGrant;

	bool IsEmpty() const
	{
		return ItemsToQuickBar.IsEmpty() && ItemsToEquip.IsEmpty() && AbilitySetsToGrant.IsEmpty();
	}
};

USTRUCT(BlueprintType)
struct FLoadoutDefinitions
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<ULoadoutItemQuickBarDef>> QuickBarDefinitions;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<ULoadoutEquipmentDefinition>> EquipmentDefinitions;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<ULoadoutAbilitiesDefinition>> AbilityDefinitions;
};

