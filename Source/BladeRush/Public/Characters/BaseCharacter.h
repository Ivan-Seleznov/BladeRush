// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/ShooterAbilitySet.h"
#include "BaseCharacter.generated.h"

class UQuickBarComponent;
class ABladeRushCameraManager;
class UEquipmentManagerComponent;
class UPlayerHealthComponent;
class UShooterMovementComponent;
class UShooterAbilitySet;
class UPlayerAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeHitPoints;
class UGameplayEffect;
class UGameplayAbility;
class UShooterCharacterData;
class UCableComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDeathStartedDelegate,class ABaseCharacter* DeadCharacter);

UCLASS()
class BLADERUSH_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	mutable FCharacterDeathStartedDelegate CharacterDeathStartedDelegate;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UPlayerAbilitySystemComponent* GetCharacterAbilitySystemComponent() const {return AbilitySystemComponent;}
	
	UFUNCTION(BlueprintCallable)
	void TryApplyAbilitySet(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet = false);

	FORCEINLINE UShooterMovementComponent* GetShooterMovementComponent() const {return ShooterMovementComponent;}
	FORCEINLINE UPlayerHealthComponent* GetPlayerHealthComponent() const {return PlayerHealthComponent;}
	FORCEINLINE UCableComponent* GetCableComponent() const {return CableComponent;}
	FORCEINLINE UEquipmentManagerComponent* GetEquipmentManagerComponent() const {return EquipmentManagerComponent;}
	FORCEINLINE UQuickBarComponent* GetQuickBarComponent() const {return QuickBarComponent;}

	ABladeRushCameraManager* GetBladeRushCameraManager() const;
	
	virtual void Jump() override;
	virtual void StopJumping() override;

	UFUNCTION(BlueprintCallable)
	virtual void Mantle();
	UFUNCTION(BlueprintCallable)
	virtual void StopMantle();
	
	UFUNCTION(Exec,Category="Commands")
	void KillYourSelf();
	UFUNCTION(Server,Reliable)
	void KillYourSelf_Server();
	
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
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void ClientPossessedBy(AController* NewController);
	
	UFUNCTION(Client, Unreliable)
	void TryApplyAbilitySet_Server(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet = false);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Movement") 
	UShooterMovementComponent* ShooterMovementComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPlayerHealthComponent* PlayerHealthComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UEquipmentManagerComponent* EquipmentManagerComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Components")
	TObjectPtr<UQuickBarComponent> QuickBarComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="GAS")
	TObjectPtr<UPlayerAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	UCableComponent* CableComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|AbiliySet")
	TObjectPtr<const UShooterAbilitySet> DefaultAbilitySet;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FTransform HandSocketTransform;
	
	// Authority-only list of granted handles
	UPROPERTY(VisibleAnywhere)
	FAbilitySet_GrantedHandles GrantedHandles;

	// Authority-only list of granted handles
	UPROPERTY(VisibleAnywhere)
	FAbilitySet_GrantedHandles DefaultGrantedHandles;	
	
	virtual void BeginPlay() override;

private:
	UFUNCTION(Reliable,Client)
	virtual void PossessedBy_Client(AController* NewController);
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void Multicast_StartDeath();
	UFUNCTION(NetMulticast,Reliable)
	virtual void Multicast_FinishDeath();
};
