// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SprintAbility.h"

#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/Attributes/AttributeStamina.h"

void USprintAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	UE_LOG(LogTemp,Warning,TEXT("INPUT RELEASED SPRINT ABILITY"));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void USprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	Character = Cast<ABaseCharacter>(ActorInfo->AvatarActor);

	UE_LOG(LogTemp,Warning,TEXT("Activate SPRINT ABILITY"));

	
	if (!Character || !SprintEffect.GameplayEffect) return;

	const UPlayerAbilitySystemComponent* PlayerAbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetComponentByClass(UPlayerAbilitySystemComponent::StaticClass()));
	if (!PlayerAbilitySystemComponent) return;

	//UE_LOG(LogTemp,Warning,TEXT("Activate SPRINT ABILITY"));

	// const UAttributeMovementSpeed* AttributeMovementSpeed = UAttributeMovementSpeed::Find(PlayerAbilitySystemComponent);
	// if (!AttributeMovementSpeed) return;
	
	if (Character->GetVelocity().Size() <= 5.0f || Character->GetMovementComponent()->IsFalling())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = /*AttributeMovementSpeed->GetWalkSpeedPoints()*/ 200;
		Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(SprintEffect.GameplayEffect, nullptr);
	}
	
	const FGameplayEffectContextHandle GameplayEffectContextHandle;
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(SprintEffect.GameplayEffect.GetDefaultObject(), SprintEffect.Level, GameplayEffectContextHandle);
		
	Character->OnCharacterMovementUpdated.AddDynamic(this, &USprintAbility::OnMovementChanged);
}

void USprintAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!Character) return;

	UE_LOG(LogTemp,Warning,TEXT("END SPRINT ABILITY"));

	const UPlayerAbilitySystemComponent* PlayerAbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetComponentByClass(UPlayerAbilitySystemComponent::StaticClass()));
	if (!PlayerAbilitySystemComponent) return;

	// const UAttributeMovementSpeed* AttributeMovementSpeed = UAttributeMovementSpeed::Find(PlayerAbilitySystemComponent);
	// if (!AttributeMovementSpeed) return;
	
	Character->OnCharacterMovementUpdated.RemoveAll(this);
	
	if (!SprintEffect.GameplayEffect) return;
	Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(SprintEffect.GameplayEffect, nullptr);
	Character->GetCharacterMovement()->MaxWalkSpeed = /*AttributeMovementSpeed->GetWalkSpeedPoints()*/200;

	Character = nullptr;
}

void USprintAbility::OnMovementChanged(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	if (!Character) return;

	UPlayerAbilitySystemComponent* PlayerAbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetComponentByClass(UPlayerAbilitySystemComponent::StaticClass()));
	if (!PlayerAbilitySystemComponent) return;

	const UAttributeStamina* AttributeStaminaPoint = UAttributeStamina::Find(Character->GetAbilitySystemComponent());
	if (!AttributeStaminaPoint) return;

	if (!SprintEffect.GameplayEffect) return;

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	if (!CharacterMovementComponent) return;

	// const UAttributeMovementSpeed* AttributeMovementSpeed = UAttributeMovementSpeed::Find(PlayerAbilitySystemComponent);
	// if (!AttributeMovementSpeed) return;
	
	
	if (Character->GetVelocity().Size() <= 5.0f || Character->GetMovementComponent()->IsFalling())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = /*AttributeMovementSpeed->GetWalkSpeedPoints()*/ 200;
		Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(SprintEffect.GameplayEffect, nullptr);
	}

	float NewSpeed = /*AttributeMovementSpeed->GetRunSpeedPoints()*/ 500;
	if (Character->GetVelocity().Size() > 0.0f)
	{
		const FGameplayEffectContextHandle GameplayEffectContextHandle;
		PlayerAbilitySystemComponent->ApplyGameplayEffectToSelf(SprintEffect.GameplayEffect.GetDefaultObject(), SprintEffect.Level, GameplayEffectContextHandle);
		
		
		if (AttributeStaminaPoint->GetStaminaPoints() <= 0.0f)
		{
			NewSpeed = /*AttributeMovementSpeed->GetWalkSpeedPoints()*/200;
	
			PlayerAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(SprintEffect.GameplayEffect, nullptr);
			Character->OnCharacterMovementUpdated.RemoveAll(this);
		}

		Character->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}
