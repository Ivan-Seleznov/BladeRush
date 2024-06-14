// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageCauserInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageCauserInterface : public UInterface
{
	GENERATED_BODY()
};

class BLADERUSH_API IDamageCauserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UTexture2D* GetCauserIcon() const = 0;
};
