// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Equipment/EquipmentInstance.h"
#include "MagazineItemInstance.generated.h"


/**
 * 
 */
UCLASS()
class BLADERUSH_API UMagazineItemInstance : public UEquipmentInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmoCount() const {return CurrentAmmoCount;}
	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmoCount() const {return MaxAmmoCount;}

	UFUNCTION(BlueprintPure)
	FGameplayTag GetAmmoType() const {return AmmoTypeTag;}

	
	UFUNCTION(BlueprintCallable)
	int32 AddCartridgeToMagazine(const int32& CartridgeCount); //Returns the remaining ammo from Cartridge Count
	UFUNCTION(BlueprintCallable)
	int32 RemoveCartridge(); //Returns the remaining cartridges in the magazine

	UFUNCTION(BlueprintPure)
	bool IsOutOfAmmo() const {return bOutOfAmmo;}

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "")
	int32 CurrentAmmoCount;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "")
	int32 MaxAmmoCount = 8.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "")
	FGameplayTag AmmoTypeTag;

private:
	bool bOutOfAmmo;
};
