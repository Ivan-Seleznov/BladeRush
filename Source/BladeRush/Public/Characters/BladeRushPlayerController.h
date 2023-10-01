// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BladeRushPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API ABladeRushPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server,Reliable)
	void TrySetPlayerPlay_Server();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool TrySetPlayerPlay();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerSpectate();

	bool IsSpectating() const {return StateName == NAME_Spectating;}
	
private:
	UFUNCTION(Reliable, Client)
	void HUDStateChanged_Client(EHUDState NewState);

	void OnRespawnTimerEnd();
	
	FTimerHandle RespawnTimerHandle;
};
