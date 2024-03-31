// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "CableComponent.h"
#include "Camera/BladeRushCameraManager.h"
#include "Characters/Components/PlayerHealthComponent.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMods/BladeRushGameMode.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/Attributes/AttributeHitPoints.h"
#include "Inventory/QuickBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponItemInstance.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UShooterMovementComponent>(CharacterMovementComponentName))
{
	ShooterMovementComponent = Cast<UShooterMovementComponent>(GetCharacterMovement());
	ShooterMovementComponent->SetIsReplicated(true);
	
	AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	PlayerHealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>("HealthComponent");
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(FName("EquipmentManagerComponent"));
	
	CableComponent = CreateDefaultSubobject<UCableComponent>("Cable");
	CableComponent->SetupAttachment(GetMesh(),FName("hand_r"));

	QuickBarComponent = CreateDefaultSubobject<UQuickBarComponent>("QuickBarComponent");
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->NavAgentProps.bCanJump = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!AbilitySystemComponent) return;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	PossessedBy_Client(NewController);
	TryApplyAbilitySet(DefaultAbilitySet);
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//client gas init

	//if (!AbilitySystemComponent) return;
	
	//AbilitySystemComponent->InitAbilityActorInfo(this,this);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::TryApplyAbilitySet(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet)
{
	if (bCancelEarlySet)
	{
		GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
	}
	
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles);
	}
}

ABladeRushCameraManager* ABaseCharacter::GetBladeRushCameraManager() const
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		return Cast<ABladeRushCameraManager>(PC->PlayerCameraManager);
	}

	return nullptr;
}

void ABaseCharacter::Jump()
{
	Super::Jump();
	bPlayerPressedJump = true;

	bPressedJump = false;
}

void ABaseCharacter::StopJumping()
{
	Super::StopJumping();

	bPlayerPressedJump = false;
}

void ABaseCharacter::Mantle()
{
	ShooterMovementComponent->Safe_bWantsToMantle = true;
}

void ABaseCharacter::StopMantle()
{
	ShooterMovementComponent->Safe_bWantsToMantle = false;
}

void ABaseCharacter::KillYourSelf()
{
	KillYourSelf_Server();
}

void ABaseCharacter::KillYourSelf_Server_Implementation()
{
	ABladeRushGameMode* GameMode = Cast<ABladeRushGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode)
	{
		return;
	}
	
	GameMode->CharacterDied(this);
}

void ABaseCharacter::OnDeathStarted()
{
	if (HasAuthority())
	{
		if (CharacterDeathStartedDelegate.IsBound())
		{
			CharacterDeathStartedDelegate.Broadcast(this);
		}
		Multicast_StartDeath();
	}
}

void ABaseCharacter::OnDeathFinished()
{
	if (HasAuthority())
	{
		Multicast_FinishDeath();
	}
}


void ABaseCharacter::StartSprinting()
{
	if (ShooterMovementComponent)
	{
		if (ShooterMovementComponent->CanSprint())
		{
			ShooterMovementComponent->Safe_bWantsToSprint = true;
		}
	}
}

void ABaseCharacter::StopSprinting()
{
	if (ShooterMovementComponent)
	{
		ShooterMovementComponent->Safe_bWantsToSprint = false;
	}
}

void ABaseCharacter::StartSliding()
{
	if (ShooterMovementComponent)
	{
		ShooterMovementComponent->Safe_bWantsToSlide = true;
		ShooterMovementComponent->bWantsToCrouch = true;
	}
}

bool ABaseCharacter::IsSliding() const
{
	if (ShooterMovementComponent)
	{
		return ShooterMovementComponent->IsCustomMovementMode(CMOVE_Slide);
	}
	
	return false;
}

FVector ABaseCharacter::GetStartGrapplingHookLocation() const
{
	return GetActorLocation();
}

FVector ABaseCharacter::GetGrapplingHookForwardVector() const
{
	return GetActorForwardVector().GetSafeNormal();
}

FCollisionQueryParams ABaseCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//TODO: Sheet code. Find a solution and remove
	UWeaponItemInstance* WeaponItemInstance = Cast<UWeaponItemInstance>(EquipmentManagerComponent->GetFirstInstanceOfType(UWeaponItemInstance::StaticClass()));
	if (WeaponItemInstance)
	{
		WeaponItemInstance->Tick(DeltaSeconds);
	}
}

void ABaseCharacter::ClientPossessedBy(AController* NewController)
{
}

void ABaseCharacter::TryApplyAbilitySet_Server_Implementation(const UShooterAbilitySet* AbilitySet,
                                                              bool bCancelEarlySet)
{
	// Clear all
	if (bCancelEarlySet)
	{
		//GetAbilitySystemComponent()->ClearAllAbilities();
		//GetAbilitySystemComponent()->RemoveAllSpawnedAttributes();

		GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
		
		//FGameplayEffectQuery Query;
		//GetAbilitySystemComponent()->RemoveActiveEffects(Query);
	}
	
	if (AbilitySet)
	{
		UE_LOG(LogTemp,Warning,TEXT("TRY APPLY SERVER"))

		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles);
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CableComponent->SetVisibility(false);
}

void ABaseCharacter::PossessedBy_Client_Implementation(AController* NewController)
{
	ClientPossessedBy(NewController);
}

void ABaseCharacter::Multicast_StartDeath_Implementation()
{
	if (CharacterDeathStartedDelegate.IsBound())
	{
		CharacterDeathStartedDelegate.Broadcast(this);
	}

	ShooterMovementComponent->ResetAllMovementActions();
	ShooterMovementComponent->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ABaseCharacter::Multicast_FinishDeath_Implementation()
{
	Destroy();
}
