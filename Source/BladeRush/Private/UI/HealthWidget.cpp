// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthWidget.h"
#include "Characters/BaseCharacter.h"
#include "Components/ProgressBar.h"
#include "GAS/Attributes/AttributeHealth.h"


void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!HealthPointsBar) return;
	
	BindHealthPointsChangeDelegate();

	isWidgetVisible = true;
}

void UHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//DisplayLog_Server();
}

void UHealthWidget::OnPawnInitialize()
{
	Super::OnPawnInitialize();
	BindHealthPointsChangeDelegate();
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
	
	if (const UAttributeHealth* AttributeHealthPoints = UAttributeHealth::Find(OwnerCharacter->GetAbilitySystemComponent()))
	{
		float Percent = AttributeHealthPoints->GetHealthPoints() / AttributeHealthPoints->GetMaxHealthPoints();
		Percent = FMath::Clamp(Percent, 0.f, 1.f);
		HealthPointsBar->SetPercent(Percent);
	}
}

void UHealthWidget::BindHealthPointsChangeDelegate()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;
	
	if (!OwnerCharacter->GetAbilitySystemComponent()) return;

	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UAttributeHealth::GetHealthPointsAttribute()).AddUObject(this, &ThisClass::HandleHealthPointsChanged);
	SetFromHealthPointsAttribute();
}

