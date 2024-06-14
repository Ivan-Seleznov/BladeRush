// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseButton.h"
#include "Components/Button.h"
#include "SelectableButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeSelection, bool, bSelected);

UCLASS()
class BLADERUSH_API USelectableButton : public UBaseButton
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ChangeSelection(const bool bInSelected);
	
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void SetSelectedStyle(const FButtonStyle& InSelectedStyle) { 	SelectedStyle = InSelectedStyle; }

	UPROPERTY(BlueprintAssignable)
	mutable FOnChangeSelection OnChangeSelection;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	FButtonStyle SelectedStyle;

	UPROPERTY(BlueprintReadOnly)
	bool bSelected;

	virtual FReply HandleClick() override;
};