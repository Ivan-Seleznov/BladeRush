// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/DeathScreenWidget.h"

#include "BladeRushLogs.h"
#include "Characters/BladeRushPlayerController.h"
#include "Characters/Components/PlayerHealthComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "UI/BaseButton.h"
#include "UI/PlayerHUD/DeathInformationWidget.h"

void UDeathScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!PrevPlayerButton || !NextPlayerButton)
	{
		return;
	}
	PrevPlayerButton->OnClicked.AddDynamic(this, &ThisClass::OnPrevPlayerClicked);
	NextPlayerButton->OnClicked.AddDynamic(this, &ThisClass::OnNextPlayerClicked);
	
	OnNativeVisibilityChanged.AddUObject(this, &ThisClass::OnVisibilityChanged);

	BindOnOutOfHealthDelegate(GetOwningPlayer()->GetPawn());
}

void UDeathScreenWidget::OnPawnInitialize(APawn* NewPawn)
{
	Super::OnPawnInitialize(NewPawn);
	
	bCanRespawn = false;
}

void UDeathScreenWidget::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPawnChanged(OldPawn, NewPawn);

	BindOnOutOfHealthDelegate(NewPawn);
}

void UDeathScreenWidget::OnVisibilityChanged(ESlateVisibility NewVisibility)
{
	if (NewVisibility == ESlateVisibility::Visible)
	{
		if (IDamageCauserInterface* DamageCauser = Cast<IDamageCauserInterface>(CurrentDeadPlayerInfo.Causer))
		{
			DeathInformationWidget->SetData(FText::FromName(CurrentDeadPlayerInfo.KillerName), DamageCauser->GetCauserIcon());
			if (PlayerNameTextBlock)
			{
				PlayerNameTextBlock->SetText(FText::GetEmpty());
			}
		}
	}
}

void UDeathScreenWidget::OnDeathFinished(AActor* OwningActor, const FDeadPlayerInfo& DeadPlayerInfo)
{
	CurrentDeadPlayerInfo = DeadPlayerInfo;
}

void UDeathScreenWidget::OnPrevPlayerClicked()
{
	if (APlayerController* PlayerController = GetOwningPlayer<APlayerController>())
	{
		PlayerController->ServerViewPrevPlayer();
		OnSpectatingPlayerChanged(PlayerController);
	}
}

void UDeathScreenWidget::OnNextPlayerClicked()
{
	if (APlayerController* PlayerController = GetOwningPlayer<APlayerController>())
	{
		PlayerController->ServerViewNextPlayer();
		OnSpectatingPlayerChanged(PlayerController);
	}
}

void UDeathScreenWidget::OnSpectatingPlayerChanged(const APlayerController* PlayerController)
{
	if (APawn* Pawn = Cast<APawn>(PlayerController->GetViewTarget()))
	{
		if(APlayerState* PlayerState = Pawn->GetPlayerState<APlayerState>())
		{
			if (PlayerNameTextBlock)
			{
				PlayerNameTextBlock->SetText(FText::FromString(PlayerState->GetPlayerName()));
			}
		}
	}
}

void UDeathScreenWidget::BindOnOutOfHealthDelegate(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}
	
	if (PrevPlayerHealthComponent)
	{
		PrevPlayerHealthComponent->OnDeathFinished.RemoveAll(this);
	}
	
	if (UPlayerHealthComponent* PlayerHealthComponent = Pawn->FindComponentByClass<UPlayerHealthComponent>())
	{
		PlayerHealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
		PrevPlayerHealthComponent = PlayerHealthComponent;
	}
}
