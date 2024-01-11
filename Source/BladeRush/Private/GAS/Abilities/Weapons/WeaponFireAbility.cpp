// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Weapons/WeaponFireAbility.h"

#include "BladeRushLogs.h"
#include "Animation/BladeRushAnimInstance.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/BaseWeaponActor.h"
#include "Weapons/WeaponItemInstance.h"

void UWeaponFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController) return;

	UWeaponItemInstance* WeaponItemInstance = GetWeaponInstance();
	if (!WeaponItemInstance) return;

	const bool bCanFire = CanFire(WeaponItemInstance,Character,PlayerController);
	if (!bCanFire)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo,false,true);
		return;
	}

	//const FGameplayEffectContextHandle EffectHandle;
	//ActiveFireEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(FireEffect.GameplayEffect.GetDefaultObject(),FireEffect.Level,EffectHandle);
	
	//if (Character->HasAuthority())
	//{
		TArray<FHitResult> OutHits;
		const FWeaponTraceData StartWeaponTraceData = CalculateStartWeaponTraceData(Character,PlayerController,WeaponItemInstance);
		WeaponFire(ActorInfo,StartWeaponTraceData,WeaponItemInstance,OutHits);
		
		//ApplyShotGameplayEffect(ActorInfo,WeaponInstance->GetBulletsPerCartridge());
	//}
#pragma region DEDUG
	if (Character->HasAuthority())
	{
		DEBUG_LOG("FireTime SERVER: %f",WeaponItemInstance->GetCurrentFireRate())
	}
	else if(Character->IsLocallyControlled())
	{
		DEBUG_LOG("FireTime CLIENT: %f",WeaponItemInstance->GetCurrentFireRate())
	}
#pragma endregion
	
	WeaponItemInstance->UpdateFiringTime();
	
	if (IsLocallyControlled())
	{
		ABaseWeaponActor* WeaponActor = WeaponItemInstance->GetSpawnedWeaponActor();
		WeaponActor->OnFire(WeaponItemInstance);
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo,false,true);
}

bool UWeaponFireAbility::CanFire(const UWeaponItemInstance* WeaponInstance, const ABaseCharacter* Character,
	const APlayerController* PlayerController) const
{
	//TODO: check ammo
	return WeaponInstance && WeaponInstance->CanFire();
}

void UWeaponFireAbility::WeaponFire(const FGameplayAbilityActorInfo* ActorInfo,
	const FWeaponTraceData& StartWeaponTraceData, UWeaponItemInstance* WeaponInstance, TArray<FHitResult> Impacts)
{
	TArray<FHitResult> Hits;
	for (int i = 0; i < WeaponInstance->GetBulletsPerCartridge(); i++)
	{
		Hits.Add(SingleBulletFire(StartWeaponTraceData,WeaponInstance,Impacts));
	}

	WeaponInstance->AddRecoil();
	
	ABaseWeaponActor* WeaponActor = WeaponInstance->GetSpawnedWeaponActor();
	if (WeaponActor)
	{
		WeaponActor->OnHit(WeaponInstance,Hits);
	}

	if (Hits.Num() > 0)
	{
		for(const FHitResult& Hit : Hits)
		{
			//TODO: reg hits
			//ApplyWeaponFire(Hit,WeaponInstance->GetWeaponVisualData().GameplayCueTag);
		}
	}	
}

FHitResult UWeaponFireAbility::SingleBulletFire(const FWeaponTraceData& StartWeaponTraceData, UWeaponItemInstance* WeaponInstance,OUT TArray<FHitResult> Impacts)
{
	FVector CameraTraceEndLoc = StartWeaponTraceData.CameraLocation + StartWeaponTraceData.CameraDirection * WeaponInstance->GetMaxTraceDistance();
	
	FHitResult CameraTraceHit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(CameraTraceHit,StartWeaponTraceData.CameraLocation,CameraTraceEndLoc,ECC_Visibility,GetCharacterFromActorInfo()->GetIgnoreCharacterParams());

	if (bEnableDebug)
	{
		DrawDebugLine(GetWorld(),StartWeaponTraceData.CameraLocation,CameraTraceEndLoc,FColor::Green,false,0.5f,0,1.f);
	}

	FVector CameraTraceHitLocation;
	if (bHit)
	{
		CameraTraceHitLocation = CameraTraceHit.Location;
	}
	else
	{
		CameraTraceHitLocation = CameraTraceHit.TraceEnd;
	}
	
	const float CameraDistance = FVector::Distance(CameraTraceHitLocation,StartWeaponTraceData.WeaponLocation);
	const FVector SpreadVector = WeaponInstance->CalculateBulletSpread(CameraDistance);

	FVector BaseEndTrace = StartWeaponTraceData.WeaponLocation + (UKismetMathLibrary::GetDirectionUnitVector(StartWeaponTraceData.WeaponLocation,CameraTraceHitLocation) * WeaponInstance->GetMaxTraceDistance());
	FVector EndWeaponTrace =  BaseEndTrace + SpreadVector;
	
DEBUG_LOG("Trace WithoutSpread: %s | EndWeapTrace: %s | SpreadVector: %s",*BaseEndTrace.ToString(),*EndWeaponTrace.ToString(),*SpreadVector.ToString());
	GetWorld()->LineTraceMultiByChannel(Impacts,StartWeaponTraceData.WeaponLocation,EndWeaponTrace,ECC_Visibility,GetCharacterFromActorInfo()->GetIgnoreCharacterParams());

	if (bEnableDebug)
	{
		DrawDebugLine(GetWorld(),StartWeaponTraceData.WeaponLocation,EndWeaponTrace,FColor::Yellow,false,0.5f,0,1.f);
	}

	
	FHitResult Hit(ForceInit);
	if (Impacts.Num() > 0)
	{
		Hit = Impacts.Last();
		DrawDebugPoint(GetWorld(),Hit.Location,4.f,FColor::Red,false,3.f);
	}
	else
	{
		Hit.bBlockingHit = false;
	}

	return Hit;
}

FWeaponTraceData UWeaponFireAbility::CalculateStartWeaponTraceData(ABaseCharacter* Character,
	APlayerController* PlayerController, UWeaponItemInstance* WeaponInstance) const
{
	const ABaseWeaponActor* WeaponActor = WeaponInstance->GetSpawnedWeaponActor();

	FWeaponTraceData StartWeaponTraceData;

	StartWeaponTraceData.WeaponLocation = WeaponActor->GetMuzzleLocation();
	if (StartWeaponTraceData.WeaponLocation == FVector::ZeroVector)
	{
		StartWeaponTraceData.WeaponLocation = WeaponActor->GetActorLocation();
	}
	
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(/*out*/ StartWeaponTraceData.CameraLocation, /*out*/ CamRot);
	StartWeaponTraceData.CameraDirection = CamRot.Vector().GetSafeNormal();

	return StartWeaponTraceData;
}
