// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "UI/BaseWidget.h"
#include "DeathScreenWidget.generated.h"

class UTextBlock;
class UBaseButton;
class UDeathInformationWidget;
class UPlayerHealthComponent;

UCLASS()
class BLADERUSH_API UDeathScreenWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void ActivateRespawn() { bCanRespawn = true; }
	
protected:
	virtual void NativeConstruct() override;
	virtual void OnPawnInitialize(APawn* NewPawn) override;

	virtual void OnPawnChanged(APawn* OldPawn, APawn* NewPawn) override;
	UFUNCTION()
	void OnVisibilityChanged(ESlateVisibility NewVisibility);

	UFUNCTION()
	void OnDeathFinished(AActor* OwningActor, const FDeadPlayerInfo& DeadPlayerInfo);

	UFUNCTION()
	void OnPrevPlayerClicked();

	UFUNCTION()
	void OnNextPlayerClicked();

	void OnSpectatingPlayerChanged(const APlayerController* PlayerController);
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDeathInformationWidget> DeathInformationWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseButton> PrevPlayerButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseButton> NextPlayerButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerNameTextBlock;

	UPROPERTY()
	UPlayerHealthComponent* PrevPlayerHealthComponent;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCanRespawn = false;

	FDeadPlayerInfo CurrentDeadPlayerInfo;

private:
	void BindOnOutOfHealthDelegate(APawn* Pawn);
};
