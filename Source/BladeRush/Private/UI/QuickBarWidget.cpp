// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickBarWidget.h"

#include "Components/HorizontalBox.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/QuickBarComponent.h"
#include "Inventory/Fragments/InventoryItemInfoFragment.h"
#include "UI/QuickBarSlotWidget.h"

void UQuickBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UQuickBarComponent* QuickBarComponent = GetQuickBarComponent();
	checkf(QuickBarComponent,TEXT("Quickbar component does not exist"));

	int32 SlotsWidgetCount = QuickBarComponent->GetMaxSlotsCount();
	QuickBarSlotWidgets.AddDefaulted(SlotsWidgetCount - QuickBarSlotWidgets.Num());
	for (int32 i = 0; i < SlotsWidgetCount; i++)
	{
		QuickBarSlotWidgets[i] = CreateWidget<UQuickBarSlotWidget>(this,QuickBarSlotWidgetClass);
		HorizontalBox->AddChild(QuickBarSlotWidgets[i]);
	}

	QuickBarComponent->OnItemAddedToSlot.AddDynamic(this,&UQuickBarWidget::OnItemAddedToSlot);
	//QuickBarComponent->OnItemRemovedFromSlot.AddDynamic(this,&UQuickBarWidget::OnItemRemovedFromSlot);
	QuickBarComponent->OnActiveSlotIndexChanged.AddDynamic(this,&UQuickBarWidget::OnQuickBarIndexChanged);
}

void UQuickBarWidget::OnQuickBarIndexChanged(int32 NewActiveSlotIndex, const UInventoryItemInstance* ItemInstance)
{
	if (ActiveSlotWidget)
	{
		ActiveSlotWidget->SetSlotIsActive(false);
	}
	
	if (!QuickBarSlotWidgets.IsValidIndex(NewActiveSlotIndex)) return;
	UQuickBarSlotWidget* SlotWidget = QuickBarSlotWidgets[NewActiveSlotIndex];
	if (!SlotWidget) return;
	
	ActiveSlotWidget = SlotWidget;
	ActiveSlotWidget->SetSlotIsActive(true);
}

void UQuickBarWidget::OnItemAddedToSlot(int32 NewActiveSlotIndex, FInventoryItemInfo InventoryItemInfo)
{
	if (!QuickBarSlotWidgets.IsValidIndex(NewActiveSlotIndex)) return;

	UQuickBarSlotWidget* SlotWidget = QuickBarSlotWidgets[NewActiveSlotIndex];
	if (!SlotWidget) return;

	SlotWidget->SetSlotItemInfo(InventoryItemInfo);
}

/*void UQuickBarWidget::OnItemRemovedFromSlot(int32 NewActiveSlotIndex, const UInventoryItemInstance* ItemInstance)
{
	if (!QuickBarSlotWidgets.IsValidIndex(NewActiveSlotIndex) || !ItemInstance) return;

	UQuickBarSlotWidget* SlotWidget = QuickBarSlotWidgets[NewActiveSlotIndex];
	if (!SlotWidget) return;

	HorizontalBox->RemoveChild(SlotWidget);
	QuickBarSlotWidgets[NewActiveSlotIndex] = nullptr;
}*/

UQuickBarComponent* UQuickBarWidget::GetQuickBarComponent()
{
	if (!GetOwningPlayerPawn()) return nullptr;

	return GetOwningPlayerPawn()->FindComponentByClass<UQuickBarComponent>();
}
