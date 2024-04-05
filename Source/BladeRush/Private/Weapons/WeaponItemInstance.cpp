// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponItemInstance.h"

#include "BladeRushLogs.h"
#include "Animation/BladeRushAnimInstance.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/BaseWeaponActor.h"
#include "Weapons/MagazineItemInstance.h"

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

	if (BaseCharacter->HasAuthority() || BaseCharacter->IsLocallyControlled())
	{
		if (!CurrentMagazine)
		{
			CurrentMagazine = FindNewMagazineInstance();
		}
	}
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
	CurrentMagazine = nullptr;
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

	const bool bInMantle = MovementComponent->GetCurrentMovementAction() == EMovementAction::Mantle;
	return IsFireRateValid() && !bInMantle && !MovementComponent->Safe_bWantsToSprint;
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

FVector2D UWeaponItemInstance::CalculateBulletSpread(const float& Distance)
{
	if (BaseBulletSpreadAngle == 0)
	{
		return FVector2d::ZeroVector;
	}
	
	FVector2D BulletSpreadValue = FVector2d::ZeroVector;

	if (/*IsInADS() &&*/ !IsMoving() && GetCurrentFireRate() < AppliedSpreadFireRate)
	{
		return BulletSpreadValue;
	}
	
	float BulletSpreadAngle = BaseBulletSpreadAngle;
	if (!IsInADS())
	{
		BulletSpreadAngle *= HipFireSpreadModifier;
	}
	if (IsMoving())
	{
		BulletSpreadAngle *= MovingSpreadModifier;
	}
	
	const float Radius = FMath::Tan(FMath::DegreesToRadians(BulletSpreadAngle)) * MaxTraceDistance;
	BulletSpreadValue = RandPointInCircle(Radius);
	BulletSpreadValue.X += GetCurrentFireRate() * FireRateSpreadModifier;
	BulletSpreadValue.Y += GetCurrentFireRate() * FireRateSpreadModifier;

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

UMagazineItemInstance* UWeaponItemInstance::FindNewMagazineInstance() const
{
	ABaseCharacter* BaseCharacter = GetBaseCharacter();
	if (!BaseCharacter || !MagazineType) return nullptr;

	UEquipmentManagerComponent* EquipmentManager = BaseCharacter->GetEquipmentManagerComponent();
	if (!EquipmentManager) return nullptr;
	
	UMagazineItemInstance* MagazineItemInstance = Cast<UMagazineItemInstance>(EquipmentManager->GetFirstInstanceOfType(MagazineType));
	return MagazineItemInstance;
}

void UWeaponItemInstance::SetNewMagazineItemInstance(UMagazineItemInstance* MagazineItemInstance)
{
	CurrentMagazine = MagazineItemInstance;
}

void UWeaponItemInstance::AddRecoil()
{
	ABaseCharacter* BaseCharacter = GetBaseCharacter();
	if (!BaseCharacter) return;

	UBladeRushAnimInstance* BladeRushAnimInstance = Cast<UBladeRushAnimInstance>(BaseCharacter->GetMesh()->GetLinkedAnimLayerInstanceByClass(WeaponVisualData.EquipAnimInstanceClass));
	if (!BladeRushAnimInstance) return;

	if (IsInADS())
	{
		BladeRushAnimInstance->AddRecoil(ADSWeaponRecoilData);
	}
	else
	{
		BladeRushAnimInstance->AddRecoil(WeaponRecoilData);
	}
	bShouldApplyRecoilControl = true;

	//for debug. delete later
	K2_Test_AddRecoil(BaseCharacter);
}

void UWeaponItemInstance::RemoveCartridge()
{
	if (CurrentMagazine)
	{
		CurrentMagazine->RemoveCartridge();
	}
}

void UWeaponItemInstance::Tick(const float& DeltaTime)
{
	ABaseCharacter* BaseCharacter = GetBaseCharacter();
	if (!BaseCharacter) return;

	APlayerController* PC = Cast<APlayerController>(BaseCharacter->GetController());
	if (!PC)
	{
		return;
	}

	if (ABaseWeaponActor* WeaponActor = GetSpawnedWeaponActor())
	{
		BaseCharacter->SetHandSocketTransform(WeaponActor->GetHandSocketTransform());
	}

	if (BaseCharacter->IsLocallyControlled())
	{
		if (RecoilControlCurve && RecoilControlTimeElapsed < RecoilControlDuration && bShouldApplyRecoilControl)
		{
			float Target = RecoilControlCurve->GetFloatValue(RecoilControlTimeElapsed) * RecoilControlLerpMultiplier + PC->GetControlRotation().Pitch;
			RecoilValue = UKismetMathLibrary::FInterpTo( PC->GetControlRotation().Pitch,Target,DeltaTime,InterpSpeed) * RecoilControlLerpMultiplier;
			RecoilControlTimeElapsed += DeltaTime;
			
			DEBUG_LOG("REcouil value: %f, TARGET: %f, CurveVal", RecoilValue, Target);

			PC->SetControlRotation(FRotator(RecoilValue,PC->GetControlRotation().Yaw,PC->GetControlRotation().Roll));

			if (RecoilControlTimeElapsed >= RecoilControlDuration)
			{
				bShouldApplyRecoilControl = false;
				RecoilControlTimeElapsed = 0.f;
				RecoilValue = 0.f;
			}
		}
	}
}

void UWeaponItemInstance::OnEnterADS()
{
	bIsInADS = true;
	ABaseWeaponActor* WeaponActor = GetSpawnedWeaponActor();
	if (WeaponActor)
	{
		WeaponActor->OnEnterADS(this);
	}
}

void UWeaponItemInstance::OnExitADS()
{
	bIsInADS = false;
	ABaseWeaponActor* WeaponActor = GetSpawnedWeaponActor();
	if (WeaponActor)
	{
		WeaponActor->OnExitADS(this);
	}
}

float UWeaponItemInstance::GetWeaponDamage(float Distance) const
{
	float DamageMultiplier = 1.f;
	if (DamageDistanceCurve)
	{
		DamageMultiplier = DamageDistanceCurve->GetFloatValue(Distance);
	}
	else
	{
		DEBUG_LOG("GetWeaponDamage does not have damage distance curve. Damage will be equal to MaxWeaponDamage");
	}
	
	return MaxWeaponDamage * DamageMultiplier;
}

float UWeaponItemInstance::GetCriticalDamageChance() const
{
	return CriticalDamageChance;
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

FVector2D UWeaponItemInstance::RandPointInCircle(float Radius) const
{
	FVector2D Result = FVector2D::ZeroVector;
	const float RandomAngle = FMath::RandRange(0.f,360.f);
	const float DistanceFromCircle = FMath::RandRange(0.f,Radius);

	const float AngleRadians = FMath::DegreesToRadians(RandomAngle);
	Result.X = DistanceFromCircle * FMath::Cos(AngleRadians);
	Result.Y = DistanceFromCircle * FMath::Sin(AngleRadians);
	return Result;
}

