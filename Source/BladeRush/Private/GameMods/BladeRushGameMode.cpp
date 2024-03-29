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

void ABladeRushGameMode::RespawnCharacterFromSpectator(APlayerController* Controller)
{
	APawn* SpectatorPawn = Controller->GetPawnOrSpectator();

	FVector PlayerSpawnLocation = FVector::ZeroVector;
	if (!PlayerStarts.IsEmpty())
	{
		PlayerSpawnLocation = PlayerStarts[UKismetMathLibrary::RandomIntegerInRange(0,PlayerStarts.Num() -1)]->GetActorLocation();;
	}
	
	ABaseCharacter* BaseCharacter = GetWorld()->SpawnActor<ABaseCharacter>(DefaultPawnClass, PlayerSpawnLocation, FRotator::ZeroRotator);
	Controller->Possess(BaseCharacter);
}

