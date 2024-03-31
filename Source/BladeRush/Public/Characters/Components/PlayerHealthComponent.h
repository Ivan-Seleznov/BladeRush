// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/GameFrameworkComponent.h"
#include "PlayerHealthComponent.generated.h"


class UAttributeHitPoints;
class ABaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealth_DeathEvent, AActor*, OwningActor);

/**
 * 
 */
UCLASS()
class BLADERUSH_API UPlayerHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
	
public:
	UPlayerHealthComponent(const FObjectInitializer& ObjectInitializer);

	mutable FHealth_DeathEvent OnDeathFinished;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	bool IsCharacterDead() const {return bIsDead;}
	
protected:
	UFUNCTION(Client,Reliable)
	void CharacterDeath_Client(ABaseCharacter* Character);
	
private:
	virtual void OnOutOfHealth(float OldValue);

	UPROPERTY()
	const UAttributeHitPoints* HealthAttributeSet;

	UPROPERTY(Replicated)
	bool bIsDead = false;
};
