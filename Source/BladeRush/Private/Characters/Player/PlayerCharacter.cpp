// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Components/ShooterHeroComponent.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/PlayerAbilitySystemComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HeroComponent = CreateDefaultSubobject<UShooterHeroComponent>(TEXT("HeroComponent"));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetMesh(), "head");

	CameraComponent->bUsePawnControlRotation = true;
	ShooterMovementComponent->bOrientRotationToMovement = false;
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
	//@TODO Remove this sheet. Move to PlayerController
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime,false);
	}
	
	//UE_LOG(LogTemp,Display,TEXT("Character max speed: %f"), GetShooterMovementComponent()->GetMaxSpeed());
	if (const UShooterMovementComponent* MoveComp = GetShooterMovementComponent())
	{
		GEngine->AddOnScreenDebugMessage(-1,-1,FColor::Yellow,FString::Printf(TEXT("Player Acceleration.Normal2D %s"),*MoveComp->GetCurrentAcceleration().GetSafeNormal2D().ToString()));
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	/*Init player native input & ability input*/
	HeroComponent->InitializePlayerInput(EnhancedInput);
}