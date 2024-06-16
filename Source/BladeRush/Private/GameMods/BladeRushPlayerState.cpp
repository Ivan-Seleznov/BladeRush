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

void ABladeRushPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if (UBladeRushGameInstance* BladeRushGameInstance = Cast<UBladeRushGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		APlayerController* PlayerController = GetPlayerController();
		if (!PlayerController)
		{
			return;
		}
		const FString& Name = BladeRushGameInstance->GetNickName();
		if (!HasAuthority())
		{
			SetPlayerName_Server(Name);
		}
		else if(PlayerController->IsLocalController() && HasAuthority())
		{
			if (Name.IsEmpty())
			{
				return;
			}
			SetPlayerName(Name);
		}
	}
}

void ABladeRushPlayerState::SetPlayerName_Server_Implementation(const FString& Name)
{
	if (Name.IsEmpty())
	{
		return;
	}
	SetPlayerName(Name);
}
