// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

class UGameplayEffect;
class ABaseCharacter;

USTRUCT(BlueprintType)
struct FShooterGameplayEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Level = 1.0f;
};

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UCLASS()
class BLADERUSH_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	
	FActiveGameplayEffectHandle ApplyGameplayEffect(const FShooterGameplayEffect& GameplayEffect);
	void RemoveGameplayEffectFromActiveHandle(FActiveGameplayEffectHandle& ActiveGameplayEffectHandle);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EAbilityActivationPolicy ActivationPolicy;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	ABaseCharacter* GetCharacterFromActorInfo() const;
	AController* GetControllerFromActorInfo() const;
};
