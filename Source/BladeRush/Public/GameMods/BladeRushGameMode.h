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

	void RespawnPawn(APlayerController* Controller);

 	FORCEINLINE float GetRespawnTimerTime() const { return RespawnTimerTime; }

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Classes")
	TSubclassOf<APawn> PawnClass;
	
private:
	void SpawnPawns();
	
	UPROPERTY(EditDefaultsOnly,Category="Gameplay",meta=(AllowPrivateAccess="true"))
	float RespawnTimerTime = 5.f;

	UPROPERTY()
	TArray<AActor*> PlayerStarts;
};
