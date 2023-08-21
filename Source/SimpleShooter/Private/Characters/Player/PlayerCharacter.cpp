// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Characters/Components/ShooterHeroComponent.h"
#include "GAS/PlayerAbilitySystemComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HeroComponent = CreateDefaultSubobject<UShooterHeroComponent>(TEXT("HeroComponent"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	/*
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
	if (InputComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerInput component is not nullptr"));
	}
	*/
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime,false);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	/*Init player native input & ability input*/
	HeroComponent->InitializePlayerInput(EnhancedInput);
}