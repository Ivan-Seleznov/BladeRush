// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMods/BladeRushPlayerState.h"

#include "BladeRushGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ABladeRushPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABladeRushPlayerState,KillCount)
	DOREPLIFETIME(ABladeRushPlayerState,DeathCount)

}

void ABladeRushPlayerState::AddKill()
{
	++KillCount;
}

void ABladeRushPlayerState::AddDeath()
{
	++DeathCount;
}

void ABladeRushPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ABladeRushPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	UBladeRushGameInstance*  BladeRushGameInstance = Cast<UBladeRushGameInstance>(UGameplayStatics::GetGameInstance(this));
	FString Name = BladeRushGameInstance->GetNickName();
	SetPlayerName(Name);
}
