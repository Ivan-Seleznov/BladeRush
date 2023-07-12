// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Triggers/BaseTrigger.h"
#include "GameplayEffectTrigger.generated.h"

/**
 * 
 */


class UGameplayEffect;

UCLASS()
class SIMPLESHOOTER_API AGameplayEffectTrigger : public ABaseTrigger
{
	GENERATED_BODY()
public:
	AGameplayEffectTrigger();
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
