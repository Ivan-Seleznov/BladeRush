// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "BladeRushSpectatorPawn.generated.h"

class UInputAction;
/**
 * Custom BladeRush Spectator Pawn
 */
UCLASS()
class BLADERUSH_API ABladeRushSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	ABladeRushSpectatorPawn() { PrimaryActorTick.bCanEverTick = true; }
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void ViewNextPlayer();
	void ViewPrevPlayer();

	void ViewSelf();

protected:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	UInputAction* ViewNextInputAction;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	UInputAction* ViewPrevInputAction;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	UInputAction* ViewSelfInputAction;
};
