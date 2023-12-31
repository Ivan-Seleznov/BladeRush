// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponItemInstance.h"

#include "BladeRushLogs.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/BaseWeaponActor.h"

UWeaponItemInstance::UWeaponItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWeaponItemInstance::OnEquipped()
{
	Super::OnEquipped();
	ABaseCharacter* BaseCharacter = GetBaseCharacter();
	if (!BaseCharacter)
	{
		return;
	}
	BaseCharacter->GetMesh()->LinkAnimClassLayers(WeaponVisualData.EquipAnimInstanceClass);
}

void UWeaponItemInstance::OnUnequipped()
{
	Super::OnUnequipped();

	ABaseCharacter* BaseCharacter = GetBaseCharacter();
	if (!BaseCharacter)
	{
		return;
	}

	BaseCharacter->GetMesh()->UnlinkAnimClassLayers(WeaponVisualData.EquipAnimInstanceClass);
}

ABaseWeaponActor* UWeaponItemInstance::GetSpawnedWeaponActor() const
{
	if (SpawnedActors.IsValidIndex(SpawnedWeaponActorIndex))
	{
		return Cast<ABaseWeaponActor>(SpawnedActors[SpawnedWeaponActorIndex]);
	}

	return nullptr;
}

bool UWeaponItemInstance::CanFire() const
{
	ABaseCharacter* Character = GetBaseCharacter();
	if (!Character) return false;

	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return false;

	return IsFireRateValid() && !MovementComponent->IsFalling() && !MovementComponent->IsInMantle();
}

bool UWeaponItemInstance::IsFireRateValid() const
{
	return GetCurrentFireRate() <= FireRate;
}

float UWeaponItemInstance::GetCurrentFireRate() const
{
	const float CurrentFireRate =  1 / GetTimeSinceLastFired();
	DEBUG_LOG("CurrentFireRate: %f",CurrentFireRate);
	return CurrentFireRate;
}

float UWeaponItemInstance::GetTimeSinceLastFired() const
{
	check(GetWorld());

	const float TimeSinceLastFired = GetWorld()->TimeSince(TimeLastFired);
	return TimeSinceLastFired;
}

FVector UWeaponItemInstance::CalculateBulletSpread(const float& Distance)
{
	if (BaseBulletSpreadAngle == 0)
	{
		return FVector::ZeroVector;
	}
	
	FVector BulletSpreadValue = FVector::ZeroVector;

	if (/*IsInScope() &&*/ !IsMoving() && GetCurrentFireRate() < AppliedSpreadFireRate)
	{
		return BulletSpreadValue;
	}
	
	float BulletSpreadAngle = BaseBulletSpreadAngle;
	if (!IsInScope())
	{
		BulletSpreadAngle *= HipFireSpreadModifier;
	}
	if (IsMoving())
	{
		BulletSpreadAngle *= MovingSpreadModifier;
		GEngine->AddOnScreenDebugMessage(-1,1,FColor::Blue,"IsMoving");
	}
	
	const float Radius = FMath::Tan(FMath::DegreesToRadians(BulletSpreadAngle)) * MaxTraceDistance;
	BulletSpreadValue = RandPointInCircle(Radius);
	BulletSpreadValue.X += GetCurrentFireRate() * FireRateSpreadModifier;
	BulletSpreadValue.Z += GetCurrentFireRate() * FireRateSpreadModifier;

	FString LogString = FString::Printf(TEXT("HipfireMod: %f | MovingMod: %f | BulletSpreadAngle: %f | Radius: %f | SpreadVector: %s"),HipFireSpreadModifier,MovingSpreadModifier,BulletSpreadAngle,Radius,*BulletSpreadValue.ToString());
	DEBUG_LOG("%s",*LogString);
	
	return BulletSpreadValue;
}

void UWeaponItemInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	TimeLastFired = World->GetTimeSeconds();
}

void UWeaponItemInstance::Tick(const float& DeltaTime)
{
	ABaseCharacter* BaseCharacter = GetBaseCharacter();
	if (BaseCharacter)
	{
		if (ABaseWeaponActor* WeaponActor = GetSpawnedWeaponActor())
		{
			BaseCharacter->SetHandSocketTransform(WeaponActor->GetHandSocketTransform());
		}
	}
}

UCharacterMovementComponent* UWeaponItemInstance::GetCharacterMovementComponent() const
{
	ABaseCharacter* Character = GetBaseCharacter();
	if (!Character) return nullptr;

	return Character->GetCharacterMovement();
}

const FVector& UWeaponItemInstance::GetCharacterVelocity() const
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovementComponent();
	check(MovementComponent);

	return MovementComponent->Velocity;
}

bool UWeaponItemInstance::IsMoving() const
{
	return GetCharacterVelocity().Length() > 5.f;
}

FVector UWeaponItemInstance::RandPointInCircle(float Radius) const
{
	FVector Result = FVector::ZeroVector;
	const float RandomAngle = FMath::RandRange(0.f,360.f);
	const float DistanceFromCircle = FMath::RandRange(0.f,Radius);

	const float AngleRadians = FMath::DegreesToRadians(RandomAngle);
	Result.X = DistanceFromCircle * FMath::Cos(AngleRadians);
	Result.Z = DistanceFromCircle * FMath::Sin(AngleRadians);
	return Result;
}

