// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponActor.generated.h"

UCLASS()
class BLADERUSH_API ABaseWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeaponActor();

	FVector GetMuzzleLocation() const;
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const {return WeaponMesh;}

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;

private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName MuzzleSocketName = FName("muzzle");
};
