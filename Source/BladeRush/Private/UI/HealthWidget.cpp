// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthWidget.h"

#include "Characters/BaseCharacter.h"
#include "Components/ProgressBar.h"
#include "GAS/Attributes/AttributeHealth.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!HealthPointsBar) return;
	
	OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;

	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UAttributeHealth::GetHealthPointsAttribute()).AddUObject(this, &ThisClass::HandleHealthPointsChanged);
	HandleHealthPointsChanged(FOnAttributeChangeData());
	isWidgetVisible = true;
}

void UHealthWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHealthWidget::HandleHealthPointsChanged(const FOnAttributeChangeData& ChangeData)
{
	if (!OwnerCharacter) return;
	
	if (const UAttributeHealth* AttributeHealthPoints = UAttributeHealth::Find(OwnerCharacter->GetAbilitySystemComponent()))
	{
		float Percent = AttributeHealthPoints->GetHealthPoints() / AttributeHealthPoints->GetMaxHealthPoints();
		Percent = FMath::Clamp(Percent, 0.f, 1.f);
		HealthPointsBar->SetPercent(Percent);
	}
}

