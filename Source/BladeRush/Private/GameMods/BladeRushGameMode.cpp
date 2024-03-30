// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMods/BladeRushGameMode.h"

#include "Characters/BaseCharacter.h"
#include "Characters/BladeRushPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ABladeRushGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),PlayerStarts);

	SpawnPawns();
}

void ABladeRushGameMode::CharacterDied(ABaseCharacter* Character)
{
	if (!Character) return;
	
	ABladeRushPlayerController* PlayerController = Cast<ABladeRushPlayerController>(Character->GetController());
	if (PlayerController)
	{
		Character->OnDeathStarted();
		PlayerController->SetPlayerSpectate();
	}

	
}

void ABladeRushGameMode::RespawnPawn(APlayerController* Controller)
{
	//APawn* SpectatorPawn = Controller->GetPawnOrSpectator();

	FVector PlayerSpawnLocation = FVector::ZeroVector;
	FRotator PlayerSpawnRotation = FRotator::ZeroRotator;

	const int32 PlayerStartIndex = FMath::RandRange(0,PlayerStarts.Num() -1);
	if (!PlayerStarts.IsEmpty() && PlayerStarts.IsValidIndex(PlayerStartIndex))
	{
		const AActor* PlayerStart = PlayerStarts[PlayerStartIndex];
		PlayerSpawnLocation = PlayerStart->GetActorLocation();
		PlayerSpawnRotation = PlayerStart->GetActorForwardVector().Rotation();
	}
	
	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, PlayerSpawnLocation, PlayerSpawnRotation);
	Controller->Possess(SpawnedPawn);
}

void ABladeRushGameMode::SpawnPawns()
{
	if (!PawnClass)
	{
		return;
	}
	
	DefaultPawnClass = PawnClass;
	TArray<AActor*> PlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),PlayerControllerClass,PlayerControllers);
		
	for (AActor* Actor : PlayerControllers)
	{
		if (APlayerController* PC = Cast<APlayerController>(Actor))
		{
			RespawnPawn(PC);
		}
	}
}

