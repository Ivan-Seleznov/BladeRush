// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BladeRushPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API ABladeRushPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintAuthorityOnly)
	void AddKill();

	UFUNCTION(BlueprintAuthorityOnly)
	void AddDeath();

	UFUNCTION(BlueprintPure)
	int32 GetKillCount() const {return KillCount;}
	UFUNCTION(BlueprintPure)
	int32 GetDeathCount() const {return DeathCount;}
	
private:
	UPROPERTY(BlueprintReadOnly,Replicated,meta=(AllowPrivateAccess))
	int32 KillCount = 0;

	UPROPERTY(BlueprintReadOnly,Replicated,meta=(AllowPrivateAccess))
	int32 DeathCount = 0;
};
