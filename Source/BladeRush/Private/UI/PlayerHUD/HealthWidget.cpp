// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD/HealthWidget.h"
#include "Characters/BaseCharacter.h"
#include "Components/ProgressBar.h"
#include "GAS/Attributes/AttributeHitPoints.h"


void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!HealthPointsBar) return;
	
	BindHealthPointsChangeDelegate();
}

void UHealthWidget::OnPawnInitialize(APawn* NewPawn)
{
	Super::OnPawnInitialize(NewPawn);
	BindHealthPointsChangeDelegate();
}

void UHealthWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;
	
	if (!OwnerCharacter->GetAbilitySystemComponent()) return;

	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UAttributeHitPoints::GetHitPointsAttribute()).RemoveAll(this);
}


void UHealthWidget::HandleHealthPointsChanged(const FOnAttributeChangeData& ChangeData)
{
	SetFromHealthPointsAttribute();
}

void UHealthWidget::SetFromHealthPointsAttribute()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;

	if (!OwnerCharacter->GetAbilitySystemComponent()) return;
	
	if (const UAttributeHitPoints* AttributeHealthPoints = UAttributeHitPoints::Find(OwnerCharacter->GetAbilitySystemComponent()))
	{
		float Percent = AttributeHealthPoints->GetHitPoints() / AttributeHealthPoints->GetMaxHitPoints();
		Percent = FMath::Clamp(Percent, 0.f, 1.f);
		HealthPointsBar->SetPercent(Percent);
	}
}

void UHealthWidget::BindHealthPointsChangeDelegate()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;
	
	if (!OwnerCharacter->GetAbilitySystemComponent()) return;

	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UAttributeHitPoints::GetHitPointsAttribute()).AddUObject(this, &ThisClass::HandleHealthPointsChanged);
	SetFromHealthPointsAttribute();
}
