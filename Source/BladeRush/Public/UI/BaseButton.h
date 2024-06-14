// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "BaseButton.generated.h"

UCLASS()
class BLADERUSH_API UBaseButton : public UButton
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
protected:
	virtual FReply HandleClick();
};
