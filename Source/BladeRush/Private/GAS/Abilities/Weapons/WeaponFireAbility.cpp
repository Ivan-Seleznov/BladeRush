// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Weapons/WeaponFireAbility.h"
#include "BladeRushLogs.h"
#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/HitReactComponent.h"
#include "Data/GameTags.h"
#include "Inventory/InventoryItemInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/BaseWeaponActor.h"
#include "Weapons/WeaponItemInstance.h"


UWeaponFireAbility::UWeaponFireAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UWeaponFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (ActorInfo->IsLocallyControlledPlayer())
	{
		DEBUG_LOG("ActivateLocalAbility");
		ActivateLocalPlayerAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else if (ActorInfo->IsNetAuthority())
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		check(ASC);
		
		NotifyTargetDataReadyDelegateHandle = ASC->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::NotifyTargetDataReady);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false,true);
	}
}
void UWeaponFireAbility::ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController) return;

	UWeaponItemInstance* WeaponItemInstance = GetWeaponInstance();
	if (!WeaponItemInstance) return;

	UInventoryItemInstance* InventoryItemInstance = GetAssociatedInventoryItemInstance();
	if (!InventoryItemInstance)
	{
		return;
	}

	
	const bool bCanFire = CanFire(WeaponItemInstance,InventoryItemInstance,Character,PlayerController);
	if (!bCanFire)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false,true);
		return;
	}
	
	
	TArray<FHitResult> OutHits;
	const FWeaponTraceData StartWeaponTraceData = CalculateStartWeaponTraceData(Character,PlayerController,WeaponItemInstance);
	
	WeaponFire(ActorInfo,StartWeaponTraceData,WeaponItemInstance,OutHits);

	/*if (ABaseWeaponActor* WeaponActor = WeaponItemInstance->GetSpawnedWeaponActor())
	{
		WeaponActor->OnFire(WeaponItemInstance);
	}*/
	
	EndAbility(Handle, ActorInfo, ActivationInfo,false,false);
}

void UWeaponFireAbility::ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
	FGameplayTag ApplicationTag)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	check(ActorInfo);

	UWeaponItemInstance* WeaponInstance = GetWeaponInstance();
	check(WeaponInstance);

	UInventoryItemInstance* InventoryItemInstance = GetAssociatedInventoryItemInstance();
	check(InventoryItemInstance);
	
	
	//for client-side prediction
	if (!ActorInfo->IsNetAuthority())
	{
		return;
	}

	
	if (ActorInfo->IsNetAuthority())
	{
		if (InventoryItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineAmmo) <= 0)
		{
			DEBUG_SLOG("Not confirm hit",FColor::Red);
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),true,false);
		}
		
		InventoryItemInstance->RemoveStatTagStack(FGameTags::Get().Weapon_MagazineAmmo,1);
		
		TArray<FHitResult> Hits;
		for (uint8 i = 0; (i < TargetDataHandle.Num()) && (i < 255); ++i)
		{
			if (TargetDataHandle.Get(i))
			{
				FHitResult Hit = *TargetDataHandle.Get(i)->GetHitResult();
				Hits.Add(Hit);
				ApplyDamageToTarget(Hit,WeaponInstance);
			}
		}
		if (FireEffect.GameplayEffect)
		{
			ApplyGameplayEffect(FireEffect);
		}

		if (ABaseWeaponActor* WeaponActor = WeaponInstance->GetSpawnedWeaponActor())
		{
			WeaponActor->OnServerHit(GetWeaponInstance(),Hits);
		}
		
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),true,false);
	}
}


bool UWeaponFireAbility::CanFire(const UWeaponItemInstance* WeaponInstance,  const UInventoryItemInstance* InventoryItemInstance, const ABaseCharacter* Character,
                                 const APlayerController* PlayerController) const
{
	return WeaponInstance && WeaponInstance->CanFire() && InventoryItemInstance && InventoryItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineAmmo) > 0;
}

void UWeaponFireAbility::WeaponFire(const FGameplayAbilityActorInfo* ActorInfo,
	const FWeaponTraceData& StartWeaponTraceData, UWeaponItemInstance* WeaponInstance, TArray<FHitResult> Impacts)
{
	//UInventoryItemInstance* InventoryItemInstance = GetAssociatedInventoryItemInstance();
	//check(InventoryItemInstance);

	//InventoryItemInstance->RemoveStatTagStack(FGameTags::Get().Weapon_MagazineAmmo,1);

	UHitReactComponent* HitReactComponent = nullptr;
	if (GetCharacterFromActorInfo())
	{
		HitReactComponent = GetCharacterFromActorInfo()->FindComponentByClass<UHitReactComponent>();
	}
	
	WeaponInstance->AddRecoil(); // ?
	
	TArray<FHitResult> Hits;
	for (int i = 0; i < WeaponInstance->GetBulletsPerCartridge(); i++)
	{
		FHitResult Hit = SingleBulletFire(StartWeaponTraceData,WeaponInstance,Impacts);
		Hits.Add(Hit);
		
		WeaponInstance->UpdateFiringTime();
		WeaponInstance->RemoveCartridge();

		if (HitReactComponent && Hit.GetActor())
		{
			HitReactComponent->OnHitReactClient(Hit);
		}
	}

	ABaseWeaponActor* WeaponActor = WeaponInstance->GetSpawnedWeaponActor();
	if (WeaponActor)
	{
		WeaponActor->OnClientHit(WeaponInstance,Hits);
	}
	
	FGameplayAbilityTargetDataHandle TargetDataHandle;

	for(const FHitResult& Hit : Hits)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
		NewTargetData->HitResult = Hit;

		TargetDataHandle.Add(NewTargetData);
	}
	
	
	if (FireEffect.GameplayEffect)
	{
		ApplyGameplayEffect(FireEffect);
	}
	

	NotifyTargetDataReady(TargetDataHandle, FGameplayTag());
}

void UWeaponFireAbility::ApplyDamageToTarget(const FHitResult& Hit, const UWeaponItemInstance* WeaponInstance)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo) return;

	const IAbilitySystemInterface* TargetAbilitySystem = Cast<IAbilitySystemInterface>(Hit.GetActor());
	if (!TargetAbilitySystem)
	{
		return;
	}
	
	if (!DamageEffect.GameplayEffect)
	{
		DEBUG_ERROR_LOG("No damage effect in fire ability");
		return;
	}
	
	FGameplayEffectContextHandle DamageEffectContextHandle = ActorInfo->AbilitySystemComponent->MakeEffectContext();
	DamageEffectContextHandle.AddSourceObject(WeaponInstance);
	DamageEffectContextHandle.AddInstigator(ActorInfo->AvatarActor.Get(),WeaponInstance->GetSpawnedWeaponActor());
	DamageEffectContextHandle.AddHitResult(Hit);

	const FGameplayEffectSpecHandle DamageEffectSpec = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(DamageEffect.GameplayEffect,DamageEffect.Level,DamageEffectContextHandle);
	FGameplayEffectSpec* Spec = DamageEffectSpec.Data.Get();
	if (!Spec)
	{
		DEBUG_ERROR_LOG("Invalid DamageEffect SpecHandle");
		return;
	}
	
	const float Distance = FVector::Distance(WeaponInstance->GetSpawnedWeaponActor()->GetActorLocation(),Hit.Location);
	Spec->SetSetByCallerMagnitude(FGameTags::Get().Data_Effect_Damage, WeaponInstance->GetWeaponDamage(Distance));
	Spec->SetSetByCallerMagnitude(FGameTags::Get().Data_Chance_Crit, WeaponInstance->GetCriticalDamageChance());

	DEBUG_SLOG(FString::Printf(TEXT("AppliedDamage: %f , Distance: %f"),WeaponInstance->GetWeaponDamage(Distance),Distance),FColor::Green);
	
	ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec,TargetAbilitySystem->GetAbilitySystemComponent());
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
	const FVector2D SpreadVector = WeaponInstance->CalculateBulletSpread(CameraDistance);

	FVector BaseEndTrace = StartWeaponTraceData.WeaponLocation + (UKismetMathLibrary::GetDirectionUnitVector(StartWeaponTraceData.WeaponLocation,CameraTraceHitLocation) * WeaponInstance->GetMaxTraceDistance());

	FVector RightVector = GetCurrentActorInfo()->AvatarActor->GetActorRightVector();
	FVector UpVector = GetCurrentActorInfo()->AvatarActor->GetActorUpVector();

	FVector EndWeaponTrace = BaseEndTrace + RightVector * SpreadVector.X + UpVector * SpreadVector.Y;
	
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

void UWeaponFireAbility::NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& InData,
	FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	if (!ASC->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);  // do not replicate
		return;
	}

	// if commit fails, cancel ability
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);  // replicate cancellation
		return;
	}

	// true if we need to replicate this target data to the server
	const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority();

	// Start a scoped prediction window
	FScopedPredictionWindow	ScopedPrediction(ASC);

	// Lyra does this memcopy operation; const cast paranoia is real. We'll keep it.
	// Take ownership of the target data to make sure no callbacks into game code invalidate it out from under us
	const FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

	// if this isn't the local player on the server, then notify the server
	if (bShouldNotifyServer)
	{
		ASC->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), LocalTargetDataHandle, ApplicationTag, ASC->ScopedPredictionKey);
	}
	
	// Execute the ability we've now successfully committed
	ActivateAbilityWithTargetData(LocalTargetDataHandle, ApplicationTag);

	// We've processed the data, clear it from the RPC buffer
	ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}


void UWeaponFireAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	// clean up the mess we made in ASC
	ASC->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).Remove(NotifyTargetDataReadyDelegateHandle);
	ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
