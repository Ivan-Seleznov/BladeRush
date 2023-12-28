11// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Weapons/BaseWeaponAbility.h"

#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/BaseWeaponActor.h"
#include "Weapons/WeaponItemInstance.h"

bool UBaseWeaponAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                            const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!GetWeaponInstance())
	{
		UE_LOG(LogTemp,Error,TEXT("Weapon ability does not have weapon instance"));
		//return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UBaseWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	
	const FWeaponTraceData WeaponTraceData = GetWeaponTraceData(PlayerController,WeaponItemInstance);
	if (Character->HasAuthority())
	{
		WeaponTrace(WeaponTraceData,WeaponItemInstance);
		
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
	}
}

UWeaponItemInstance* UBaseWeaponAbility::GetWeaponInstance() const
{
	return Cast<UWeaponItemInstance>(GetAssociatedEquipmentInstance());
}

void UBaseWeaponAbility::SingleBulletTrace(const FWeaponTraceData& TraceData)
{
	//TODO Implement
}

void UBaseWeaponAbility::WeaponTrace(const FWeaponTraceData& TraceData,UWeaponItemInstance* WeaponItemInstance)
{
	//TODO Use single bullet trace
	
	FVector CameraTraceEndLoc = TraceData.StartCamTraceLoc + TraceData.CameraDirection * Distance;
	
	FHitResult CameraTraceHit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(CameraTraceHit,TraceData.StartCamTraceLoc,CameraTraceEndLoc,ECC_Visibility,GetCharacterFromActorInfo()->GetIgnoreCharacterParams());
	DrawDebugLine(GetWorld(),TraceData.StartCamTraceLoc,CameraTraceEndLoc,FColor::Blue,false,2,0,2);
	
	if (!bHit) return;
	DrawDebugPoint(GetWorld(),CameraTraceHit.Location,5,FColor::Red,false,3);
	
	FHitResult FinalHit;//to - from
	FVector EndWeaponTrace = TraceData.WeaponTraceLoc + ((CameraTraceHit.Location - TraceData.WeaponTraceLoc));

	GetWorld()->LineTraceSingleByChannel(FinalHit,TraceData.WeaponTraceLoc,EndWeaponTrace,ECC_Visibility,GetCharacterFromActorInfo()->GetIgnoreCharacterParams());
	DrawDebugLine(GetWorld(),TraceData.WeaponTraceLoc,EndWeaponTrace,FColor::Yellow,false,2,0,3);
	
}

FWeaponTraceData UBaseWeaponAbility::GetWeaponTraceData(APlayerController* PlayerController,  UWeaponItemInstance* WeaponItemInstance)
{
	AActor* WeaponActor = WeaponItemInstance->GetSpawnedWeaponActor();
	check(WeaponActor);
	
	FVector WeaponLoc;
	if (ABaseWeaponActor* BaseWeaponActor = Cast<ABaseWeaponActor>(WeaponActor))
	{
		WeaponLoc = BaseWeaponActor->GetMuzzleLocation();
	}
	else
	{
		WeaponLoc = WeaponActor->GetActorLocation();
	}
	
	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);

	FWeaponTraceData TraceData;
	TraceData.WeaponTraceLoc = WeaponLoc;
	TraceData.StartCamTraceLoc = CamLoc;
	TraceData.CameraDirection = CamRot.Vector().GetSafeNormal();

	return TraceData;
}


