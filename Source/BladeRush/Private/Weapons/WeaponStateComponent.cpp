// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponStateComponent.h"

#include "BladeRushLogs.h"
#include "Characters/BaseCharacter.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Weapons/WeaponItemInstance.h"

UWeaponStateComponent::UWeaponStateComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ABaseCharacter* BaseCharacter = GetPawn<ABaseCharacter>();
	if (!BaseCharacter) return;

	UEquipmentManagerComponent* EquipmentManagerComponent = BaseCharacter->GetEquipmentManagerComponent();
	if (!EquipmentManagerComponent) return;

	UWeaponItemInstance* WeaponItemInstance = Cast<UWeaponItemInstance>(EquipmentManagerComponent->GetFirstInstanceOfType(UWeaponItemInstance::StaticClass()));
	if (!WeaponItemInstance) return;
	
	//WeaponItemInstance->Tick(DeltaTime);
}

