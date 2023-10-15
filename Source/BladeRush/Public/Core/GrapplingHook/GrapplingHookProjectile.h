// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrapplingHookProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class BLADERUSH_API AGrapplingHookProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrapplingHookProjectile();
	
	void FireInDirection(const FVector& ShootDirection);
	USphereComponent* GetProjectileCollisionComponent() const {return SphereCollisionComponent;}
protected:
	UPROPERTY(EditDefaultsOnly,Category="Components")
	USphereComponent* SphereCollisionComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="Components")
	UStaticMeshComponent* StaticMeshComponent;
};
