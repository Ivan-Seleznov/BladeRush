// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ShooterInputComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Data/GameTags.h"
#include "GAS/PlayerAbilitySystemComponent.h"

void UShooterInputComponent::InitializePlayerInput(UEnhancedInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent) return;
	if (!InputConfig) return;

	const ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character) return;
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MappingContext, 0);

			const FGameTags GameTags = FGameTags::Get();
			for (const FShooterInputAction& Action : InputConfig->AbilityInputActions)
			{
				TArray<uint32> BindHandles;
				BindAbilityActions(PlayerInputComponent,this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
					
				BindNativeAction(PlayerInputComponent,GameTags.InputTag_Look, ETriggerEvent::Triggered, this, &UShooterInputComponent::Look, /*bLogIfNotFound=*/ false);
				BindNativeAction(PlayerInputComponent,GameTags.InputTag_Move, ETriggerEvent::Triggered, this, &UShooterInputComponent::Move, /*bLogIfNotFound=*/ false);
			}
		}
	}
}

void UShooterInputComponent::ChangeMappingContext(UInputMappingContext* NewMappingContext)
{
	if (ABaseCharacter* Character = GetCharacter<ABaseCharacter>())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Character->Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(MappingContext);
				Subsystem->AddMappingContext(NewMappingContext,0);
			
				MappingContext = NewMappingContext;
			}
		}
	}
}

void UShooterInputComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (Character)
	{
		if (UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
		{
			AbilitySystemComponent->AbilityInputTagPressed(InputTag);
		}
	}
}

void UShooterInputComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (Character)
	{
		if (UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
		{
			AbilitySystemComponent->AbilityInputTagReleased(InputTag);
		}
	}
}

void UShooterInputComponent::Move(const FInputActionValue& Value)
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character) return;

	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Character->Controller != nullptr)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MovementVector.Y);
		Character->AddMovementInput(Character->GetActorRightVector(), MovementVector.X);
	}
}

void UShooterInputComponent::Look(const FInputActionValue& Value)
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character) return;
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Character->Controller != nullptr)
	{
		Character->AddControllerYawInput(LookAxisVector.X);
		Character->AddControllerPitchInput(LookAxisVector.Y);
	}
}
