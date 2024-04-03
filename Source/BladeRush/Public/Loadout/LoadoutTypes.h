
#pragma once
#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Loadout/LoadoutItemDefinition.h"
#include "Loadout/LoadoutAbilitiesDefinition.h"
#include "Loadout/LoadoutItemQuickBarDef.h"
#include "LoadoutTypes.generated.h"

class ULoadoutDefinitionBase;
class UEquipmentDefinition;
class UShooterAbilitySet;
class UInventoryItemDefinition;
class ULoadoutItemQuickBarDef;
class ULoadoutItemDefinition;
class ULoadoutAbilitiesDefinition;


USTRUCT(BlueprintType)
struct FLoadoutBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<ULoadoutDefinitionBase> LoadoutDefinition;
};

USTRUCT(BlueprintType)
struct FLoadoutItem : public FLoadoutBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UInventoryItemDefinition> ItemDef;
};

USTRUCT(BlueprintType)
struct FLoadoutAbilities : public FLoadoutBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UShooterAbilitySet> AbilitySet;
};


USTRUCT(BlueprintType)
struct FCharacterLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FLoadoutItem> ItemsToQuickBar;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FLoadoutItem> ItemsToEquip;

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
	TArray<TSubclassOf<ULoadoutItemDefinition>> EquipmentDefinitions;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<ULoadoutAbilitiesDefinition>> AbilityDefinitions;
};

