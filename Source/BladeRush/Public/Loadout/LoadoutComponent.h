// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Loadout/LoadoutTypes.h"
#include "Components/ControllerComponent.h"
#include "GAS/ShooterAbilitySet.h"
#include "LoadoutComponent.generated.h"


class ABaseCharacter;
class UAvailableLoadoutDataAsset;
class UEquipmentManagerComponent;
class UInventoryManagerComponent;
class UEquipmentInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLADERUSH_API ULoadoutComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	ULoadoutComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const FCharacterLoadout& GetCurrentLoadout() {return CurrentLoadout;}
	TObjectPtr<UAvailableLoadoutDataAsset> GetAvailableLoadoutDataAsset() const {return AvailableLoadoutDataAsset;}

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	void SetCurrentLoadout(const FCharacterLoadout& NewLoadout, bool bTryApplyLoadout = false);
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	bool TryApplyCurrentLoadout();

	void TrySetCurrentLoadout(const FLoadoutDefinitions& LoadoutDefinitions);
	
private:
	void ApplyQuickBarEquipment(ABaseCharacter* Character);
	void ApplyEquipment(ABaseCharacter* Character);
	void ApplyAbilities(ABaseCharacter* Character);

	void ClearPrevLoadout();
	
	void SetCurrentLoadoutFromDef(const FLoadoutDefinitions& LoadoutDefinitions);

	FLoadoutEquipment CreateLoadoutEquipmentItem(TSubclassOf<ULoadoutEquipmentDefinition> LoadoutEquipmentDefClass);
	FLoadoutQuickBarItem CreateLoadoutQuickBarItem(TSubclassOf<ULoadoutItemQuickBarDef> LoadoutItemDefClass);
	FLoadoutAbilities CreateLoadoutAbilities(TSubclassOf<ULoadoutAbilitiesDefinition> LoadoutAbilitiesDefClass);
	
	UFUNCTION(Server,Reliable)
	void SetCurrentLoadoutFromDef_Server(const FLoadoutDefinitions& LoadoutDefinitions);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UAvailableLoadoutDataAsset> AvailableLoadoutDataAsset;

	UPROPERTY(BlueprintReadOnly,Replicated,meta=(AllowPrivateAccess))
	FCharacterLoadout CurrentLoadout;

	UPROPERTY()
	TArray<UEquipmentInstance*> CurrentEquipmentInstances;
	
	FAbilitySet_GrantedHandles CurrentLoadoutGrantedHandles;
};
