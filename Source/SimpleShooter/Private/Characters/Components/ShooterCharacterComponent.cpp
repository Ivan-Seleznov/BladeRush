// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ShooterCharacterComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/ShooterCharacterData.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Data/GameTags.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "Input/ShooterInputConfig.h"

void UShooterCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
	//InitCharacter();
}

void UShooterCharacterComponent::InitCharacter()
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character)
	{
		return;
	}

	if (const UShooterCharacterData* CharacterData = Character->GetCharacterData())
	{
		
	}
	/*const UCharacterData* PawnData = nullptr;*/

	/*add ExtComp ?*/
	
	/*
	
	if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		PawnData = PawnExtComp->GetPawnData<ULyraPawnData>();

		// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
		// The ability system component and attribute sets live on the player state.
		PawnExtComp->InitializeAbilitySystem(LyraPS->GetLyraAbilitySystemComponent(), LyraPS);
	}
	
	*/

	if (Character->InputComponent && Character->Controller)
	{
		InitializePlayerInput(Character->InputComponent);
	}
}

void UShooterCharacterComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character)
	{
		return;
	}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			if (const UShooterCharacterData* CharacterData = Character->GetCharacterData())
			{
				if (const UShooterInputConfig* InputConfig = CharacterData->InputConfig)
				{
					Subsystem->AddMappingContext(MappingContext, 0);

					const FGameTags GameTags = FGameTags::Get();
					for (const FShooterInputAction& Action : InputConfig->AbilityInputActions)
					{
						TArray<uint32> BindHandles;
						BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
					
						BindNativeAction(InputConfig, GameTags.InputTag_Look, ETriggerEvent::Triggered, this, &UShooterCharacterComponent::Look, /*bLogIfNotFound=*/ false);
						BindNativeAction(InputConfig, GameTags.InputTag_Move, ETriggerEvent::Triggered, this, &UShooterCharacterComponent::Move, /*bLogIfNotFound=*/ false);
					}
				}
			}
		}
	}
}

void UShooterCharacterComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	UE_LOG(LogTemp,Display,TEXT("InputTagPressed"));
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (Character)
	{
		if (UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
		{
			AbilitySystemComponent->AbilityInputTagPressed(InputTag);
		}
	}
}

void UShooterCharacterComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
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

void UShooterCharacterComponent::Move(const FInputActionValue& Value)
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character)
	{
		return;
	}
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Character->Controller != nullptr)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MovementVector.Y);
		Character->AddMovementInput(Character->GetActorRightVector(), MovementVector.X);
	}
}

void UShooterCharacterComponent::Look(const FInputActionValue& Value)
{
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character)
	{
		return;
	}
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Character->Controller != nullptr)
	{
		Character->AddControllerYawInput(LookAxisVector.X);
		Character->AddControllerPitchInput(LookAxisVector.Y);
	}
}
