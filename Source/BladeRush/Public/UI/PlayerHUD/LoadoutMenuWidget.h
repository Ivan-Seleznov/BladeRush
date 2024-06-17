// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "LoadoutMenuWidget.generated.h"

struct FLoadoutQuickBarItem;
enum class EWeaponType : uint8;
struct FLoadoutWeapons;
class UComboBoxString;
class ULoadoutEquipmentDataAsset;
class UWeaponTypesDataAsset;
class UButton;
class ULoadoutComponent;
struct FCharacterLoadout;

UCLASS()
class BLADERUSH_API ULoadoutMenuWidget : public UBaseWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ApplyLoadout();
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UComboBoxString* PrimaryWeaponComboBox;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UComboBoxString* SecondaryWeaponComboBox;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	UComboBoxString* AbilitiesComboBox;

private:
	void LoadLoadouts(ULoadoutComponent* LoadoutComponent);
	void LoadCurrentLoadout(ULoadoutComponent* LoadoutComponent);

	FLoadoutQuickBarItem FindWeaponBySlotInCurrentLoadout(const FCharacterLoadout& CurrentLoadout,int32 SlotIndex) const;
};
