// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StaminaWidget.h"

#include "AbilitySystemComponent.h"
#include "AudioDevice.h"
#include "Characters/BaseCharacter.h"
#include "Components/ProgressBar.h"
#include "GAS/Attributes/AttributeStamina.h"


void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!StaminaPointsBar) return;
	
	BindStaminaChangedDelegate();

	bWidgetVisible = false;
	SetRenderOpacity(0);
	
}

void UStaminaWidget::HandleStaminaPointsChanged(const FOnAttributeChangeData& ChangeData)
{
	
	GetWorld()->GetTimerManager().ClearTimer(PeriodBarShowTimerHandle);

	SetFromStaminaAttribute();
	
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeSinceLastUpdate = CurrentTime - LastHPUpdateTime;
		
	if (TimeSinceLastUpdate >= 2.0f)
	{
		if (!bWidgetVisible)
		{
			bWidgetVisible = true;
			SetRenderOpacity(1);
			PlayAnimation(Fade);
		}
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]()
		{
			PlayAnimationReverse(Fade);
			bWidgetVisible = false;
		});
			
		GetWorld()->GetTimerManager().SetTimer(BarShowTimerHandle,TimerDelegate,2,false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(BarShowTimerHandle);
			
		if (!bWidgetVisible)
		{
			bWidgetVisible = true;
			SetRenderOpacity(1);
			PlayAnimation(Fade);
		}
			
		if (GetWorld()->GetTimerManager().IsTimerActive(PeriodBarShowTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PeriodBarShowTimerHandle);
		}
		else /*if it is last update - PlayAnimationReverse(Fade)*/
			{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]()
			{
				PlayAnimationReverse(Fade);
				bWidgetVisible = false;
			});
			GetWorld()->GetTimerManager().SetTimer(PeriodBarShowTimerHandle,TimerDelegate,2,false);
			}
	}
	LastHPUpdateTime = CurrentTime;
}

void UStaminaWidget::OnPawnInitialize()
{
	Super::OnPawnInitialize();
	BindStaminaChangedDelegate();
}

void UStaminaWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;

	if (!OwnerCharacter->GetAbilitySystemComponent()) return;

	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UAttributeStamina::GetStaminaPointsAttribute()).RemoveAll(this);
}

void UStaminaWidget::BindStaminaChangedDelegate()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;

	if (!OwnerCharacter->GetAbilitySystemComponent()) return;

	OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UAttributeStamina::GetStaminaPointsAttribute()).AddUObject(this, &ThisClass::HandleStaminaPointsChanged);
	SetFromStaminaAttribute();
}

void UStaminaWidget::SetFromStaminaAttribute()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;

	if (!OwnerCharacter->GetAbilitySystemComponent()) return;
	
	if (const UAttributeStamina* AttributeStaminaPoints = UAttributeStamina::Find(OwnerCharacter->GetAbilitySystemComponent()))
	{
		float Percent = AttributeStaminaPoints->GetStaminaPoints() / AttributeStaminaPoints->GetMaxStaminaPoints();
		Percent = FMath::Clamp(Percent, 0.f, 1.f);
		StaminaPointsBar->SetPercent(Percent);
	}
}
