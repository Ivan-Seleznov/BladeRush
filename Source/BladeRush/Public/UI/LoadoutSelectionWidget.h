// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "LoadoutSelectionWidget.generated.h"

struct FLoadoutItem;
enum class EWeaponType : uint8;
struct FLoadoutWeapons;
class UComboBoxString;
class ULoadoutEquipmentDataAsset;
class UWeaponTypesDataAsset;
class UButton;
class ULoadoutComponent;
struct FCharacterLoadout;

/**
 * 
 */
UCLASS()
class BLADERUSH_API ULoadoutSelectionWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnApplyButtonClicked();
	
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UButton* ApplyButton;

	/*Weapons*/
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UComboBoxString* PrimaryWeaponComboBox;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UComboBoxString* SecondaryWeaponComboBox;
	
	/*Equipment*/
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UComboBoxString* EquipmentComboBox;

	/*Abilities*/
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UComboBoxString* AbilitySetComboBox;

private:
	void LoadLoadouts(ULoadoutComponent* LoadoutComponent);
	void LoadCurrentLoadout(ULoadoutComponent* LoadoutComponent);

	FLoadoutItem FindWeaponBySlotInCurrentLoadout(const FCharacterLoadout& CurrentLoadout,int32 SlotIndex) const;
};
