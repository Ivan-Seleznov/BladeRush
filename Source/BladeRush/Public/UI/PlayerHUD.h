// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UDeathScreenWidget;

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	OnlySpectating,
	CanRespawnSpectating,
};

/**
 * Player HUD class
 */
UCLASS()
class BLADERUSH_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	void OnPlayerHudStateChanged(EHUDState HudState);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UUserWidget> PlayerHudClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UDeathScreenWidget> DeathWidgetClass;
private:
	UPROPERTY()
	UUserWidget* PlayerHud;

	UPROPERTY()
	UDeathScreenWidget* DeathWidget;

	EHUDState CurrentState;
};
