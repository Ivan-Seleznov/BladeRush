// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/BladeRushCameraManager.h"
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
	
	SpringArmComponent->SetupAttachment(GetMesh(), CameraAttachSocketName);
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	CameraComponent->bUsePawnControlRotation = true;
	ShooterMovementComponent->bOrientRotationToMovement = false;
}

void APlayerCharacter::ClientPossessedBy(AController* NewController)
{
	Super::ClientPossessedBy(NewController);

	check(NewController);

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (ABladeRushCameraManager* BladeRushCameraManager = Cast<ABladeRushCameraManager>(PC->PlayerCameraManager))
		{
			BladeRushCameraManager->InitCameraManagerPawn(this);
		}
	}
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

void APlayerCharacter::SetMouseAxisValue(const float& X, const float Y)
{
	HandSwayData.MouseX = X;
	HandSwayData.MouseY = Y;
}

void APlayerCharacter::SetMoveXValue(const float& X)
{
	HandSwayData.SideMove = X;
}
