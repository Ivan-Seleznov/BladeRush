// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LoadoutDefinitionBase.generated.h"

/**
 * Base loadout definition
 */
UCLASS(BlueprintType,Blueprintable,Abstract)
class BLADERUSH_API ULoadoutDefinitionBase : public UObject
{
	GENERATED_BODY()

public:
	UTexture* GetImage() const {return Image;}
	const FText& GetDescription() const {return Description;}
	const FName& GetDisplayName() const {return DisplayName;}
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName DisplayName;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture> Image;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText Description;
};
