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

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const;

	FHealth_DeathEvent OnDeathFinished;

	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);

protected:

	UFUNCTION(Client,Reliable)
	void CharacterDeath_Client(ABaseCharacter* Character);
private:
	void OnHitPointsChanged(const FOnAttributeChangeData& ChangeData);

	UPROPERTY()
	const UAttributeHitPoints* HealthAttributeSet;
};
