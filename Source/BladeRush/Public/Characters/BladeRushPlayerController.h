// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BladeRushPlayerController.generated.h"

class ULoadoutDataAsset;
class UInventoryManagerComponent;
class ABaseCharacter;
class ULoadoutComponent;
/**
 * 
 */
UCLASS()
class BLADERUSH_API ABladeRushPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABladeRushPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	FORCEINLINE UInventoryManagerComponent* GetInventoryManagerComponent() const {return InventoryManagerComponent;}
	
	UFUNCTION(BlueprintCallable, Server,Reliable)
	void TrySetPlayerPlay_Server();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool TrySetPlayerPlay();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerSpectate();

	bool IsSpectating() const {return StateName == NAME_Spectating;}

	virtual void AcknowledgePossession(APawn* P) override;

protected:
	virtual void BeginPlay() override;
	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	ULoadoutComponent* LoadoutComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	ULoadoutDataAsset* DefaultLoadout;
private:
	UFUNCTION(Reliable, Client)
	void HUDStateChanged_Client(EHUDState NewState);

	void OnRespawnTimerEnd();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	UInventoryManagerComponent* InventoryManagerComponent;
	
	FTimerHandle RespawnTimerHandle;

	UPROPERTY()
	ABaseCharacter* DeadCharacter;
};
