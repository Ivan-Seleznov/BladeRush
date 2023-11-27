// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickBarSlotWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Fragments/InventoryItemInfoFragment.h"

void UQuickBarSlotWidget::SetSlotItemInfo(const FInventoryItemInfo& SlotItemInfo)
{
	SlotImage->SetBrushFromTexture(SlotItemInfo.ImageTexture);
	ItemNameTextBlock->SetText(SlotItemInfo.ItemNameText);
	ItemInfoTextBlock->SetText(SlotItemInfo.ItemInfoText);
}

void UQuickBarSlotWidget::SetSlotIsActive(bool bInSlotIsActive)
{
	bSlotIsActive = bInSlotIsActive;
	SetBrushColor();
}

void UQuickBarSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetBrushColor();
}

void UQuickBarSlotWidget::SetBrushColor()
{
	FLinearColor Color = ItemBorder->GetBrushColor();
	bSlotIsActive ? Color.A = ActiveBrushOpacity : Color.A = 0.0f;
	ItemBorder->SetBrushColor(Color);
}
