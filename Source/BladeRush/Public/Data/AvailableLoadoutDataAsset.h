// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Loadout/LoadoutTypes.h"
#include "AvailableLoadoutDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UAvailableLoadoutDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	const FLoadoutDefinitions& GetLoadoutDefinitions() const {return LoadoutDefinitions;}

	bool ContainsQuickBarDef(TSubclassOf<ULoadoutItemQuickBarDef> QuickBarDef) const;
	bool ContainsEquipDef(TSubclassOf<ULoadoutItemDefinition> ItemDef) const;
	bool ContainsAbilitiesDef(TSubclassOf<ULoadoutAbilitiesDefinition> AbilitiesDef) const;
	
	const TSubclassOf<ULoadoutItemQuickBarDef>* FindQuickBarDefByName(FName InName) const;
	const TSubclassOf<ULoadoutItemDefinition>* FindEquipDefByName(FName InName) const;
	const TSubclassOf<ULoadoutAbilitiesDefinition>* FindAbilitiesDefByName(FName InName) const;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FLoadoutDefinitions LoadoutDefinitions;
};
