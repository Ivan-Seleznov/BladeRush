// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquipmentInstance.h"
#include "WeaponItemInstance.generated.h"

class UMagazineItemInstance;
class UBladeRushAnimInstance;
class UCharacterMovementComponent;
class ABaseWeaponActor;

USTRUCT(BlueprintType)
struct FWeaponRecoilData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float RecoilMultiplier;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SideRecoilMultiplier;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MoveXMultiplier;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MoveZMultiplier;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MoveYMultiplier;
};

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
	
	FVector2D CalculateBulletSpread(const float& Distance);
	bool IsInADS() const {return bIsInADS;}
	void UpdateFiringTime();

	float GetMaxTraceDistance() const {return MaxTraceDistance;}
	float GetBulletsPerCartridge() const {return BulletsPerCartridge;}

	const FWeaponRecoilData& GetWeaponRecoilData() const {return WeaponRecoilData;}
	const FWeaponVisualData& GetWeaponVisualData() const {return WeaponVisualData;}
	
	UMagazineItemInstance* GetCurrentMagazine() const {return CurrentMagazine;}
	TSubclassOf<UMagazineItemInstance> GetMagazineType() const {return MagazineType;}

	UMagazineItemInstance* FindNewMagazineInstance() const;
	void SetNewMagazineItemInstance(UMagazineItemInstance* MagazineItemInstance);
	
	void AddRecoil();
	void RemoveCartridge();
	
	void Tick(const float& DeltaTime);
	void OnEnterADS();
	void OnExitADS();
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_Test_AddRecoil(ABaseCharacter* Character);

	UFUNCTION(BlueprintPure)
	float GetWeaponDamage(float Distance) const;
	UFUNCTION(BlueprintPure)
	float GetCriticalDamageChance() const;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 SpawnedWeaponActorIndex = 0;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FWeaponVisualData WeaponVisualData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Magazine", meta=(AllowPrivateAccess=true))
	TSubclassOf<UMagazineItemInstance> MagazineType;
	UPROPERTY(BlueprintReadOnly)
	UMagazineItemInstance* CurrentMagazine;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FWeaponRecoilData WeaponRecoilData;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FWeaponRecoilData ADSWeaponRecoilData;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float ADSRecoilMultiplier = 0.5f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Weapon|Recoil")
	UCurveFloat* RecoilControlCurve;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Weapon|Recoil")
	float RecoilControlDuration = 0.15f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Weapon|Recoil")
	float RecoilControlMultiplier = 1.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Weapon|Recoil")
	float RecoilControlLerpMultiplier = 0.15f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Trace")
	float MaxTraceDistance = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Spread")
	float BaseBulletSpreadAngle;

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

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Damage")
	float MaxWeaponDamage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Weapon|Damage", meta=(UIMin=0, ClampMin=0, UIMax=1, ClampMax=1))
	float CriticalDamageChance;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Damage")
	UCurveFloat* DamageDistanceCurve;
	
private:
	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;
	
	bool bIsInADS = false;

	float RecoilControlTimeElapsed = 0.f;
	bool bShouldApplyRecoilControl = false;
	
	bool IsMoving() const;
	FVector2D RandPointInCircle(float Radius) const;

	UCharacterMovementComponent* GetCharacterMovementComponent() const;
	const FVector& GetCharacterVelocity() const;
};
