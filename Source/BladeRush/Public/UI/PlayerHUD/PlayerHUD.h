// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerHUDWidget.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UKillfeedPanelWidget;
class UPlayerHUDWidget;
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

	UKillfeedPanelWidget* GetKillfeedPanelWidget() const
	{
		return PlayerHud->GetKillfeedPanel();
	}
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UUserWidget> PlayerHudClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UDeathScreenWidget> DeathWidgetClass;
private:
	UPROPERTY()
	UPlayerHUDWidget* PlayerHud;

	UPROPERTY()
	UDeathScreenWidget* DeathWidget;

	EHUDState CurrentState;
};
