// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeaponActor.h"

ABaseWeaponActor::ABaseWeaponActor()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	RootComponent = WeaponMesh;
}

FVector ABaseWeaponActor::GetMuzzleLocation() const
{
	check(WeaponMesh);

	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}
