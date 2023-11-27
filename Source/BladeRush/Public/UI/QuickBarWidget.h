// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "QuickBarWidget.generated.h"

struct FInventoryItemInfo;
class UHorizontalBox;
class UInventoryItemInstance;
class UQuickBarComponent;
class UQuickBarSlotWidget;

UCLASS()
class BLADERUSH_API UQuickBarWidget : public UBaseWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnQuickBarIndexChanged(int32 NewActiveSlotIndex,const UInventoryItemInstance* ItemInstance);

	UFUNCTION()
	void OnItemAddedToSlot(int32 NewActiveSlotIndex,FInventoryItemInfo InventoryItemInfo);

	//UFUNCTION()
	//void OnItemRemovedFromSlot(int32 NewActiveSlotIndex,const UInventoryItemInstance* ItemInstance);
private:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* HorizontalBox;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TSubclassOf<UQuickBarSlotWidget> QuickBarSlotWidgetClass;
	
	UQuickBarComponent* GetQuickBarComponent();
	
	UPROPERTY()
	TArray<UQuickBarSlotWidget*> QuickBarSlotWidgets;

	UPROPERTY()
	UQuickBarSlotWidget* ActiveSlotWidget;
};
