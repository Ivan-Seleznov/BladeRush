// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquipmentInstance.h"
#include "WeaponItemInstance.generated.h"

class UCharacterMovementComponent;
class ABaseWeaponActor;

USTRUCT(BlueprintType)
struct FWeaponVisualData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UAnimMontage* ReloadAnimMontage;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UAnimMontage* ReloadWeaponAnimMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAnimInstance> EquipAnimInstanceClass;
	
	//UPROPERTY(EditAnywhere,BlueprintReadOnly="Decal")
	//UDecalData* DecalDataAsset;
};


/**
 * Weapon item instance class
 */
UCLASS()
class BLADERUSH_API UWeaponItemInstance : public UEquipmentInstance
{
	GENERATED_BODY()
public:
	UWeaponItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnEquipped() override;
    virtual void OnUnequipped() override;

	ABaseWeaponActor* GetSpawnedWeaponActor() const;

	bool CanFire() const;
	bool IsFireRateValid() const;
	
	float GetCurrentFireRate() const;
	float GetTimeSinceLastFired() const;
	
	FVector CalculateBulletSpread(const float& Distance);
	bool IsInScope() const {return bIsInScope;}
	void UpdateFiringTime();

	float GetMaxTraceDistance() const {return MaxTraceDistance;}
	float GetBulletsPerCartridge() const {return BulletsPerCartridge;}

	void Tick(const float& DeltaTime);
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 SpawnedWeaponActorIndex = 0;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FWeaponVisualData WeaponVisualData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Trace")
	float MaxTraceDistance = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Spread")
	float BaseBulletSpreadAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Recoil")
	float BaseRecoil = 2.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Fire")
	float FireRate = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Spread")
	float AppliedSpreadFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Spread")
	float FireRateSpreadModifier = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Bullet")
	int32 BulletsPerCartridge = 1.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Fire")
	bool bIsAutoWeapon = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Spread")
	float HipFireSpreadModifier = 3.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Scope")
	float ScopeEntryTime = 0.5f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Spread")
	float MovingSpreadModifier = 1.3f;

private:
	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;
	
	bool bIsInScope = false;

	bool IsMoving() const;
	FVector RandPointInCircle(float Radius) const;

	UCharacterMovementComponent* GetCharacterMovementComponent() const;
	const FVector& GetCharacterVelocity() const;
};
