// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BladeRushGameMode.generated.h"

class ABaseCharacter;
/**
 * 
 */
UCLASS()
class BLADERUSH_API ABladeRushGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	void CharacterDied(ABaseCharacter* Character);

	void RespawnCharacterFromSpectator(APlayerController* Controller);

 	FORCEINLINE float GetRespawnTimerTime() const { return RespawnTimerTime; }
	
private:
	UPROPERTY(EditDefaultsOnly,Category="Gameplay",meta=(AllowPrivateAccess="true"))
	float RespawnTimerTime = 5.f;


	TArray<AActor*> PlayerStarts;
};
