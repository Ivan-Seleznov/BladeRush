// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Weapons/WeaponReloadAbility.h"

#include "BladeRushLogs.h"
#include "Characters/BaseCharacter.h"
#include "Data/GameTags.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Weapons/MagazineItemInstance.h"
#include "Weapons/WeaponItemInstance.h"

void UWeaponReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!GetWeaponInstance())
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
		return;
	}
	
	const UInventoryItemInstance* ItemInstance = GetAssociatedInventoryItemInstance();
	if (!ItemInstance)
	{
		return;
	}

	const bool bHasRoomInMagazine = ItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineAmmo) < ItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineSize);
	const bool bHasSpareAmmo = ItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_SpareAmmo) > 0;

	if (!bHasRoomInMagazine || !bHasSpareAmmo)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
		return;
	}
	
	ReloadEffectActiveHandle = ApplyGameplayEffect(ReloadEffect);
	
	K2_PlayReloadAnimation();
}

void UWeaponReloadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveGameplayEffectFromActiveHandle(ReloadEffectActiveHandle);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UWeaponReloadAbility::OnWeaponReloaded()
{
	const ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character)
	{
		return;
	}

	UWeaponItemInstance* WeaponItemInstance = GetWeaponInstance();
	if (!WeaponItemInstance)
	{
		return;
	}
	
	if (UInventoryItemInstance* InventoryItemInstance = GetAssociatedInventoryItemInstance())
	{
		ReloadAmmoInMagazine(InventoryItemInstance);
	}
	else
	{
		DEBUG_LOG("Can not get associated inventory item instance");
	}

	EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),false,false);
}

void UWeaponReloadAbility::ReloadAmmoInMagazine(UInventoryItemInstance* InventoryItemInstance)
{
	const int32 MagazineSize = InventoryItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineSize);
	const int32 RemainingMagazineAmmo = InventoryItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineAmmo);
	const int32 StartingAmmoCount = InventoryItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_SpareAmmo);

	const int32 TotalAmmo = StartingAmmoCount + RemainingMagazineAmmo;
	
	const int32 AmmoToAdd = FMath::Min(MagazineSize,TotalAmmo) - RemainingMagazineAmmo;

	InventoryItemInstance->AddStatTagStack(FGameTags::Get().Weapon_MagazineAmmo,AmmoToAdd);
	InventoryItemInstance->RemoveStatTagStack(FGameTags::Get().Weapon_SpareAmmo, AmmoToAdd);
}

UMagazineItemInstance* UWeaponReloadAbility::GetMagazineItemInstance() const
{
	const UWeaponItemInstance* WeaponItemInstance = GetWeaponInstance();
	if (WeaponItemInstance)
	{
		return WeaponItemInstance->GetCurrentMagazine();
	}

	return nullptr;
}
