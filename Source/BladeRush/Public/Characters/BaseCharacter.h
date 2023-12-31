// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/ShooterAbilitySet.h"
#include "BaseCharacter.generated.h"

class UEquipmentManagerComponent;
class UPlayerHealthComponent;
class UShooterMovementComponent;
class UShooterAbilitySet;
class UPlayerAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeHealth;
class UGameplayEffect;
class UGameplayAbility;
class UShooterCharacterData;
class UCableComponent;

UCLASS()
class BLADERUSH_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UPlayerAbilitySystemComponent* GetCharacterAbilitySystemComponent() const {return AbilitySystemComponent;}
	
	UFUNCTION(BlueprintCallable)
	void TryApplyAbilitySet(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet = false);

	FORCEINLINE UShooterMovementComponent* GetShooterMovementComponent() const {return ShooterMovementComponent;}
	FORCEINLINE UPlayerHealthComponent* GetPlayerHealthComponent() const {return PlayerHealthComponent;}
	FORCEINLINE UCableComponent* GetCableComponent() const {return CableComponent;}
	FORCEINLINE UEquipmentManagerComponent* GetEquipmentManagerComponent() const {return EquipmentManagerComponent;}
	
	virtual void Jump() override;
	virtual void StopJumping() override;

	void OnDeathStarted();
	void OnDeathFinished();
	
	void StartSprinting();
	void StopSprinting();

	void StartSliding();

	UFUNCTION(BlueprintCallable)
	bool IsSliding() const;

	virtual FVector GetStartGrapplingHookLocation() const;
	virtual FVector GetGrapplingHookForwardVector() const;

	FCollisionQueryParams GetIgnoreCharacterParams() const;

	const FTransform& GetHandSocketTransform() {return HandSocketTransform;}
	void SetHandSocketTransform(const FTransform& SocketTransform) {HandSocketTransform = SocketTransform;}
	
	bool bPlayerPressedJump;
protected:

	UFUNCTION(Client, Unreliable)
	void TryApplyAbilitySet_Server(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet = false);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Movement") 
	UShooterMovementComponent* ShooterMovementComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPlayerHealthComponent* PlayerHealthComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UEquipmentManagerComponent* EquipmentManagerComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="GAS")
	TObjectPtr<UPlayerAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	UCableComponent* CableComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|AbiliySet")
	TObjectPtr<const UShooterAbilitySet> DefaultAbilitySet;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FTransform HandSocketTransform;
	
	// Authority-only list of granted handles
	UPROPERTY(VisibleAnywhere)
	FAbilitySet_GrantedHandles GrantedHandles;
	
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast,Reliable)
	virtual void Multicast_StartDeath();
	UFUNCTION(NetMulticast,Reliable)
	virtual void Multicast_FinishDeath();
};
