// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HitMarkerWidgetBase.h"

#include "Animation/WidgetAnimation.h"

void UHitMarkerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UHitMarkerWidgetBase::AddHitMarker()
{
	SetVisibility(ESlateVisibility::Visible);
	if (HitMarkerFadeOutAnimation)
	{
		PlayAnimation(HitMarkerFadeOutAnimation);
	}
}

void UHitMarkerWidgetBase::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == HitMarkerFadeOutAnimation)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHitMarkerWidgetBase::OnPawnInitialize(APawn* NewPawn)
{
	Super::OnPawnInitialize(NewPawn);
}
