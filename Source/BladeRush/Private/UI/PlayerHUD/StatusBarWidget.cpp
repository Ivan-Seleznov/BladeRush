// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/StatusBarWidget.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Data/GameTags.h"
#include "GAS/Attributes/AttributeHitPoints.h"
#include "Inventory/QuickBarComponent.h"

void UStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!HealthBar || !AmmoCountTextBlock)
	{
		return;
	}
	BindHealthPointsChangeDelegate();
	RefreshAmmoCounter();
}

void UStatusBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter || !OwnerCharacter->GetAbilitySystemComponent())
	{
		return;
	}
	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		UAttributeHitPoints::GetHitPointsAttribute()).RemoveAll(this);
}

void UStatusBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!QuickBarComponentCached)
	{
		return;
	}

	UInventoryItemInstance* InventoryItemInstance = QuickBarComponentCached->GetCurrentInventoryItemInstance();
	if (!InventoryItemInstance)
	{
		return;
	}
	CurrentAmmoInClip = InventoryItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineAmmo);
	CurrentMaxAmmoInClip = InventoryItemInstance->GetStatTagStackCount(FGameTags::Get().Weapon_MagazineSize);

	AmmoCountTextBlock->SetText(FText::Format(NSLOCTEXT("StatusBar","AmmoCount","{0}/{1}"),
		FText::AsNumber(CurrentAmmoInClip), FText::AsNumber(CurrentMaxAmmoInClip)));
}

void UStatusBarWidget::OnPawnInitialize(APawn* NewPawn)
{
	Super::OnPawnInitialize(NewPawn);
	
	BindHealthPointsChangeDelegate();
	RefreshAmmoCounter();
}

void UStatusBarWidget::BindHealthPointsChangeDelegate()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter || !OwnerCharacter->GetAbilitySystemComponent())
	{
		return;
	}

	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		UAttributeHitPoints::GetHitPointsAttribute()).AddUObject(this, &ThisClass::HandleHealthPointsChanged);
	UpdateHealthDisplay();
}

void UStatusBarWidget::UpdateHealthDisplay()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter || !OwnerCharacter->GetAbilitySystemComponent())
	{
		return;
	}
	if (const UAttributeHitPoints* AttributeHealthPoints = UAttributeHitPoints::Find(OwnerCharacter->GetAbilitySystemComponent()))
	{
		float CurrentHealthPoints =  AttributeHealthPoints->GetHitPoints();
		float Percent = CurrentHealthPoints / AttributeHealthPoints->GetMaxHitPoints();
		Percent = FMath::Clamp(Percent, 0.f, 1.f);
		HealthBar->SetPercent(Percent);

		HealthCountTextBlock->SetText(FText::Format(NSLOCTEXT("StatusBar","HealthPoints","{0}+"), FText::AsNumber(CurrentHealthPoints)));
	}
}

void UStatusBarWidget::RefreshAmmoCounter()
{
	AmmoCountTextBlock->SetText(FText::Format(NSLOCTEXT("StatusBar","AmmoCount","{0}/{1}"),
		FText::AsNumber(CurrentAmmoInClip), FText::AsNumber(CurrentMaxAmmoInClip)));
	if(APawn* Pawn = GetOwningPlayer()->GetPawn())
	{
		UQuickBarComponent* QuickBarComponent = Cast<UQuickBarComponent>(Pawn->GetComponentByClass(UQuickBarComponent::StaticClass()));
		if (!QuickBarComponent)
		{
			return;
		}
		QuickBarComponentCached = QuickBarComponent;
	}
}

void UStatusBarWidget::HandleHealthPointsChanged(const FOnAttributeChangeData& ChangeData)
{
	UpdateHealthDisplay();
}
