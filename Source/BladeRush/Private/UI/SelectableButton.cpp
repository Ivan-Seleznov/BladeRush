// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectableButton.h"

void USelectableButton::ChangeSelection(const bool bInSelected)
{
	bSelected = bInSelected;
	if (MyButton)
	{
		MyButton->SetButtonStyle(bSelected ? &SelectedStyle : &WidgetStyle);
	}
	OnChangeSelection.Broadcast(bSelected);
}

TSharedRef<SWidget> USelectableButton::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	
	MyButton->SetButtonStyle(bSelected ? &SelectedStyle : &WidgetStyle);
	
	return Widget;
}

FReply USelectableButton::HandleClick()
{
	ChangeSelection(true);
	return Super::HandleClick();
}