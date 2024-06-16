﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BladeRushGameInstance.generated.h"

UCLASS()
class BLADERUSH_API UBladeRushGameInstance : public UGameInstance
{
  GENERATED_BODY()
  
public:
  const FString& GetNickName() { return NickName; }
  void SetNickName(const FString& InNickName) { NickName = InNickName; }
  
private:
  UPROPERTY()
  FString NickName;
};
