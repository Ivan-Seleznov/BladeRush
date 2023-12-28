// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EquipmentInstance.generated.h"

struct FEquipmentActorToSpawn;

/**
 * EquipmentInstance class
 */
UCLASS(BlueprintType,Blueprintable)
class BLADERUSH_API UEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override;
	//~End of UObject interface

	APawn* GetPawn() const;
	
	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	UFUNCTION(BlueprintCallable, Category=Equipment)
	void SetInstigator(UObject* InInstigator);

	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() const {return Instigator;}

	
	virtual void SpawnEquipmentActors(const TArray<FEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();
	
	virtual void OnEquipped();
	virtual void OnUnequipped();
	
protected:
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
private:
	UPROPERTY(Replicated)
	TObjectPtr<UObject> Instigator;
	
};
