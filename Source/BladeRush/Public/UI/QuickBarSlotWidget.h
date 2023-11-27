// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "QuickBarSlotWidget.generated.h"

struct FInventoryItemInfo;
class UBorder;
class UTextBlock;
class UImage;

UCLASS()
class BLADERUSH_API UQuickBarSlotWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void SetSlotItemInfo(const FInventoryItemInfo& SlotItemInfo);
	void SetSlotIsActive(bool bInSlotIsActive);

	bool IsSlotActive() const {return bSlotIsActive;}
	void DeleteWidget() {RemoveFromParent();}
protected:
	virtual void NativeConstruct() override;
private:

	void SetBrushColor();
	
	UPROPERTY(meta = (BindWidget))
	UImage* SlotImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemInfoTextBlock;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess))
	float ActiveBrushOpacity = 0.5f;
	
	bool bSlotIsActive = false;
};
