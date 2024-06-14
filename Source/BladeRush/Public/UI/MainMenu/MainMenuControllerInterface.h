// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MainMenuControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMainMenuControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class BLADERUSH_API IMainMenuControllerInterface
{
	GENERATED_BODY()

public:
	virtual void ReturnToMainMenu() = 0;
};
