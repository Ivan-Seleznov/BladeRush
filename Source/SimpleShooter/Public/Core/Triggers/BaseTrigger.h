// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrigger.generated.h"

class UBoxComponent;

UCLASS(Abstract)
class SIMPLESHOOTER_API ABaseTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTrigger();
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;
};
