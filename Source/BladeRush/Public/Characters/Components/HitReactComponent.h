// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PawnComponent.h"
#include "HitReactComponent.generated.h"


struct FGameplayEffectContextHandle;
struct FGameplayTagContainer;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLADERUSH_API UHitReactComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UHitReactComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual void OnHitReactClient(const FHitResult& HitResult);
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHitReact(FHitResult HitResult,FGameplayTagContainer SourceTags,FGameplayEffectContextHandle GameplayEffectSpec);

	UFUNCTION(Unreliable,NetMulticast)
	virtual void Multicast_OnHitReact(FHitResult HitResult,FVector HitDirection);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnHitReact(const FHitResult& HitResult, const FVector& HitDirection);
	
private:
	UFUNCTION(BlueprintPure)
	FVector GetHitDirection(const FHitResult& HitResult) const;
};
