// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * Player HUD class
 */
UCLASS()
class BLADERUSH_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UUserWidget> PlayerHudClass;
	
private:
	UPROPERTY() UUserWidget* PlayerHud;
};
