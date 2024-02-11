// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ShooterHeroComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Data/GameTags.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "Inventory/QuickBarComponent.h"

void UShooterHeroComponent::InitializePlayerInput(UEnhancedInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent) return;
	if (!InputConfig) return;

	const ABaseCharacter* Character = GetPawn<ABaseCharacter>(); //GetCharacter<ABaseCharacter>();
	if (!Character) return;
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MappingContext, 0);

			const FGameTags GameTags = FGameTags::Get();
			TArray<uint32> BindHandles;
			
			BindAbilityActions(PlayerInputComponent,this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
				
			BindNativeAction(PlayerInputComponent,GameTags.InputTag_Look, ETriggerEvent::Triggered, this, &UShooterHeroComponent::Look, /*bLogIfNotFound=*/ false);
			BindNativeAction(PlayerInputComponent,GameTags.InputTag_Move, ETriggerEvent::Triggered, this, &UShooterHeroComponent::Move, /*bLogIfNotFound=*/ false);

			BindNativeAction(PlayerInputComponent,GameTags.InputTag_QuickBar_Slot0, ETriggerEvent::Triggered, this, &UShooterHeroComponent::Input_QuickBarSlot0, /*bLogIfNotFound=*/ false);
			BindNativeAction(PlayerInputComponent,GameTags.InputTag_QuickBar_Slot1, ETriggerEvent::Triggered, this, &UShooterHeroComponent::Input_QuickBarSlot1, /*bLogIfNotFound=*/ false);
			BindNativeAction(PlayerInputComponent,GameTags.InputTag_QuickBar_Slot2, ETriggerEvent::Triggered, this, &UShooterHeroComponent::Input_QuickBarSlot2, /*bLogIfNotFound=*/ false);
			BindNativeAction(PlayerInputComponent,GameTags.InputTag_QuickBar_Slot3, ETriggerEvent::Triggered, this, &UShooterHeroComponent::Input_QuickBarSlot3, /*bLogIfNotFound=*/ false);
		}
	}
}

void UShooterHeroComponent::ChangeMappingContext(UInputMappingContext* NewMappingContext)
{
	if (ABaseCharacter* Character = GetPawn<ABaseCharacter>())
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

void UShooterHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	const ABaseCharacter* Character = GetPawn<ABaseCharacter>();
	if (Character)
	{
		if (UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
		{
			AbilitySystemComponent->AbilityInputTagPressed(InputTag);
		}
	}
}

void UShooterHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const ABaseCharacter* Character = GetPawn<ABaseCharacter>();
	if (Character)
	{
		if (UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
		{
			AbilitySystemComponent->AbilityInputTagReleased(InputTag);
		}
	}
}

void UShooterHeroComponent::Move(const FInputActionValue& Value)
{
	ABaseCharacter* Character = GetPawn<ABaseCharacter>();
	if (!Character) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Character->Controller != nullptr)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MovementVector.Y);
		Character->AddMovementInput(Character->GetActorRightVector(), MovementVector.X);

		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
		{
			PlayerCharacter->SetMoveXValue(MovementVector.X);
		}
	}
}

void UShooterHeroComponent::Look(const FInputActionValue& Value)
{
	ABaseCharacter* Character = GetPawn<ABaseCharacter>();
	if (!Character) return;
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>() * 0.5f;
	if (Character->Controller != nullptr)
	{
		Character->AddControllerYawInput(LookAxisVector.X);
		Character->AddControllerPitchInput(LookAxisVector.Y);
		
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
		{
			PlayerCharacter->SetMouseAxisValue(LookAxisVector.X,LookAxisVector.Y);
		}
	}
}

void UShooterHeroComponent::Input_QuickBarSlot1(const FInputActionValue& Value)
{
	InventorySlotChanged(1);
}

void UShooterHeroComponent::Input_QuickBarSlot2(const FInputActionValue& Value)
{
	InventorySlotChanged(2);

}

void UShooterHeroComponent::Input_QuickBarSlot3(const FInputActionValue& Value)
{
	InventorySlotChanged(3);
}

void UShooterHeroComponent::Input_QuickBarSlot0(const FInputActionValue& Value)
{
	InventorySlotChanged(0);
}

void UShooterHeroComponent::InventorySlotChanged(const float& SlotValue)
{
	//if (CurrentSlotIndex == SlotValue) return;
	
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	UQuickBarComponent* QuickBarComponent = Pawn->FindComponentByClass<UQuickBarComponent>();
	if (!QuickBarComponent) return;
	
	const float SlotIndex = SlotValue - 1;
	if (SlotIndex >= QuickBarComponent->GetMaxSlotsCount()) return;
	
	GEngine->AddOnScreenDebugMessage(-1,1,FColor::White,FString::Printf(TEXT("Slot index: %f"),SlotIndex));

	QuickBarComponent->SetActiveSlotIndex(SlotIndex);
	//CurrentSlotIndex = SlotIndex;
}
