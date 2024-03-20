// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "BladeRushAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FShooterGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
 
protected:
	UPROPERTY()
	bool bIsCriticalHit;
 
public:
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
 
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FShooterGameplayEffectContext* Duplicate() const override
	{
		FShooterGameplayEffectContext* NewContext = new FShooterGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
 
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
};
 
template <>
struct TStructOpsTypeTraits<FShooterGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FShooterGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};