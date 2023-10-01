// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMods/BladeRushGameMode.h"

#include "Characters/BaseCharacter.h"
#include "Characters/BladeRushPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ABladeRushGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),PlayerStarts);

	if (PlayerStarts[0])
	{
		PlayerStartLocation = PlayerStarts[0]->GetActorLocation();
	}
}

void ABladeRushGameMode::CharacterDied(ABaseCharacter* Character)
{
	if (!Character) return;
	
	ABladeRushPlayerController* PlayerController = Cast<ABladeRushPlayerController>(Character->GetController());
	if (PlayerController)
	{
		Character->Destroy(true,true);
		PlayerController->SetPlayerSpectate();
	}

	
}

void ABladeRushGameMode::RespawnCharacterFromSpectator(APlayerController* Controller)
{
	APawn* SpectatorPawn = Controller->GetPawnOrSpectator();

	
	ABaseCharacter* BaseCharacter = GetWorld()->SpawnActor<ABaseCharacter>(DefaultPawnClass, PlayerStartLocation, FRotator::ZeroRotator);
	Controller->Possess(BaseCharacter);
}
