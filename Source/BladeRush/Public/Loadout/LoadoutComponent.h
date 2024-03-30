// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Loadout/LoadoutTypes.h"
#include "Components/ControllerComponent.h"
#include "LoadoutComponent.generated.h"


class ABaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLADERUSH_API ULoadoutComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	ULoadoutComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void SetCurrentLoadout(const FCharacterLoadout& NewLoadout, bool bTryApplyLoadout = false);
	bool TryApplyCurrentLoadout();

	UFUNCTION(BlueprintPure)
	const FCharacterLoadout& GetCurrentLoadout() {return CurrentLoadout;}
private:
	void ApplyQuickBarEquipment(ABaseCharacter* Character);
	void ApplyEquipment(ABaseCharacter* Character);
	void ApplyAbilities(ABaseCharacter* Character);

	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess))
	FCharacterLoadout CurrentLoadout;
};
