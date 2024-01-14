// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponAbility.h"
#include "WeaponFireAbility.generated.h"

class UWeaponItemInstance;

USTRUCT()
struct FWeaponTraceData
{
	GENERATED_BODY()

	FVector CameraLocation;
	FVector CameraDirection;
	
	FVector WeaponLocation;
};

/**
 * Base weapon ability class
 */
UCLASS()
class BLADERUSH_API UWeaponFireAbility : public UBaseWeaponAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual bool CanFire(const UWeaponItemInstance* WeaponInstance, const ABaseCharacter* Character,const APlayerController* PlayerController) const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FShooterGameplayEffect FireEffect;
	
	void WeaponFire(const FGameplayAbilityActorInfo* ActorInfo,const FWeaponTraceData& StartWeaponTraceData, UWeaponItemInstance* WeaponInstance,OUT TArray<FHitResult> Impacts);
	
	FHitResult SingleBulletFire(const FWeaponTraceData& StartWeaponTraceData, UWeaponItemInstance* WeaponInstance,OUT TArray<FHitResult> Impacts);
	FWeaponTraceData CalculateStartWeaponTraceData(ABaseCharacter* Character,APlayerController* PlayerController, UWeaponItemInstance* WeaponInstance) const;
private:
	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> InvItemInstance;
};
