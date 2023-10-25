// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrapplingHookProjectile.generated.h"

class UCableComponent;
class USphereComponent;
class UProjectileMovementComponent;
class UMovementAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FProjectileDestroyed,AActor*/*Owner*/);

UCLASS()
class BLADERUSH_API AGrapplingHookProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrapplingHookProjectile();

	virtual void Tick(float DeltaSeconds) override;
	
	USphereComponent* GetProjectileCollisionComponent() const {return SphereCollisionComponent;}

	//FORCEINLINE float GetMaxDistance() const {return MaxDistance;}
	//FORCEINLINE void SetMaxDistance(float NewRange) {MaxDistance = NewRange;}
	
	FProjectileDestroyed OnProjectileDestroyed;
protected:
	UPROPERTY(EditDefaultsOnly,Category="Components")
	USphereComponent* SphereCollisionComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="Components")
	UStaticMeshComponent* StaticMeshComponent;

	//UPROPERTY(EditDefaultsOnly)
	//float MaxDistance = 1500.0f;

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	FVector ProjectileStartLocation;

	UPROPERTY() const UMovementAttributeSet* MovementAttributeSet;
};
