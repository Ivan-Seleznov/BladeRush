// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Weapons/WeaponReloadAbility.h"

#include "Characters/BaseCharacter.h"
#include "Equipment/EquipmentManagerComponent.h"
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
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;
	
	UWeaponItemInstance* WeaponItemInstance = GetWeaponInstance();
	if (!WeaponItemInstance) return;
	
	UMagazineItemInstance* MagazineItemInstance = GetMagazineItemInstance();
	
	if (MagazineItemInstance && MagazineItemInstance->IsOutOfAmmo())
	{
		//TODO: Expose to MagazineManager component?
		UEquipmentManagerComponent* EquipmentManagerComponent = Character->GetEquipmentManagerComponent();
		if (!EquipmentManagerComponent) return;

		EquipmentManagerComponent->UnequipItem(MagazineItemInstance);
	}
	UMagazineItemInstance* NewMagazineItemInstance = WeaponItemInstance->FindNewMagazineInstance();
	WeaponItemInstance->SetNewMagazineItemInstance(NewMagazineItemInstance);

	EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),false,false);
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
