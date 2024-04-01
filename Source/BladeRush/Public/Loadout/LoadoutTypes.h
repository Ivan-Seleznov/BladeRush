
#pragma once
#include "CoreMinimal.h"
#include "LoadoutTypes.generated.h"

class UEquipmentDefinition;
class UShooterAbilitySet;
class UInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FCharacterLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TMap<int32,TSubclassOf<UInventoryItemDefinition>> ItemsToQuickBar;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UEquipmentDefinition>> ItemsToEquip;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UShooterAbilitySet* AbilityToGrant;

	bool IsEmpty() const
	{
		return ItemsToQuickBar.IsEmpty() && ItemsToEquip.IsEmpty() && !AbilityToGrant;
	}
};

