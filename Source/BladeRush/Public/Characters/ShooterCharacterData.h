// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShooterCharacterData.generated.h"

class UShooterInputConfig;
class UShooterAbilitySet;
class ABaseCharacter;
/**
 * 
 */
UCLASS()
class BLADERUSH_API UShooterCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UShooterCharacterData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Pawn")
	TSubclassOf<ABaseCharacter> CharacterClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Abilities")
	TArray<TObjectPtr<UShooterAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Abilities")
	//TObjectPtr<ULyraAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Input")
	TObjectPtr<UShooterInputConfig> InputConfig;
};
