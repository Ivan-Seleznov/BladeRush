// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GameplayAbilityFromEquipment.h"
#include "BaseWeaponAbility.generated.h"

class UWeaponItemInstance;

USTRUCT()
struct FWeaponTraceData
{
	GENERATED_BODY()

	FVector StartCamTraceLoc;
	FVector WeaponTraceLoc;
	FVector CameraDirection;
};

/**
 * Base weapon ability class
 */
UCLASS()
class BLADERUSH_API UBaseWeaponAbility : public UGameplayAbilityFromEquipment
{
	GENERATED_BODY()
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UWeaponItemInstance* GetWeaponInstance() const;

protected:
	void SingleBulletTrace(const FWeaponTraceData& TraceData);
	void WeaponTrace(const FWeaponTraceData& TraceData,UWeaponItemInstance* WeaponItemInstance);
	
	FWeaponTraceData GetWeaponTraceData(APlayerController* PlayerController,  UWeaponItemInstance* WeaponItemInstance);

	
private:
	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> InvItemInstance;
};
