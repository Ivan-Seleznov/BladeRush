// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BladeRushButton.generated.h"

class UBaseButton;
class USizeBox;
class UButton;

USTRUCT(BlueprintType)
struct FButtonSettings
{
  GENERATED_BODY()
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
  FVector2D ButtonSize = FVector2D(32.f, 32.f);
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
  FButtonStyle Style;
};
UCLASS()
class BLADERUSH_API UBladeRushButton : public UUserWidget
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  UBaseButton* GetActualButton() const { return ActualButton; }
  
protected:
  virtual void NativePreConstruct() override;
  
  UPROPERTY(meta=(BindWidget))
  UBaseButton* ActualButton;

  UPROPERTY(meta=(BindWidget))
  USizeBox* ButtonSizeBox;

  UPROPERTY(EditAnywhere)
  FButtonSettings ButtonSettings;
};
