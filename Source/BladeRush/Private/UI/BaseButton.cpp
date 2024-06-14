// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseButton.h"

FReply UBaseButton::HandleClick()
{
	OnClicked.Broadcast();

	return FReply::Handled();
}

TSharedRef<SWidget> UBaseButton::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	
	MyButton->SetOnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, HandleClick));
	
	return Widget;
}