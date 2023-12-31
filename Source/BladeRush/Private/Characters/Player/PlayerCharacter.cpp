// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Components/ShooterHeroComponent.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HeroComponent = CreateDefaultSubobject<UShooterHeroComponent>(TEXT("HeroComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArmComponent"));
	WeaponPreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponPreviewMesh"));
	WeaponPreviewMesh->SetupAttachment(GetMesh(),AttachSocketName);
	//QuickBarComponent = CreateDefaultSubobject<UQuickBarComponent>(FName("QuickBarComponent"));
	
	SpringArmComponent->SetupAttachment(GetMesh(), "head");
	SpringArmComponent->SetupAttachment(GetMesh(), "head");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	CameraComponent->bUsePawnControlRotation = true;
	ShooterMovementComponent->bOrientRotationToMovement = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponPreviewMesh)
	{
		WeaponPreviewMesh->DestroyComponent();
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	/*Init player native input & ability input*/
	HeroComponent->InitializePlayerInput(EnhancedInput);
}

FVector APlayerCharacter::GetStartGrapplingHookLocation() const
{
	if (!CameraComponent) return FVector::ZeroVector;

	return CameraComponent->GetComponentLocation();
}

FVector APlayerCharacter::GetGrapplingHookForwardVector() const
{
	if (!CameraComponent) return FVector::ZeroVector;
	
	return CameraComponent->GetForwardVector();
}

