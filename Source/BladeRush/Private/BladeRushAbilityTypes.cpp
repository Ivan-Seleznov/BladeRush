// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeRushAbilityTypes.h"

bool FShooterGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	 enum RepFlag
    {
        REP_Instigator,
        REP_EffectCauser,
        REP_AbilityCDO,
        REP_SourceObject,
        REP_Actors,
        REP_HitResult,
        REP_WorldOrigin,
        REP_IsCriticalHit,
        REP_MAX
    };
     
    uint16 RepBits = 0;
    if (Ar.IsSaving())
    {
        if (Instigator.IsValid())
        {
            RepBits |= 1 << REP_Instigator;
        }
        if (EffectCauser.IsValid())
        {
            RepBits |= 1 << REP_EffectCauser;
        }
        if (AbilityCDO.IsValid())
        {
            RepBits |= 1 << REP_AbilityCDO;
        }
        if (bReplicateSourceObject && SourceObject.IsValid())
        {
            RepBits |= 1 << REP_SourceObject;
        }
        if (Actors.Num() > 0)
        {
            RepBits |= 1 << REP_Actors;
        }
        if (HitResult.IsValid())
        {
            RepBits |= 1 << REP_HitResult;
        }
        if (bHasWorldOrigin)
        {
            RepBits |= 1 << REP_WorldOrigin;
        }
        if (bIsCriticalHit)
        {
            RepBits |= 1 << REP_IsCriticalHit;
        }
    }
 
    Ar.SerializeBits(&RepBits, REP_MAX);
 
    if (RepBits & (1 << REP_Instigator))
    {
        Ar << Instigator;
    }
    if (RepBits & (1 << REP_EffectCauser))
    {
        Ar << EffectCauser;
    }
    if (RepBits & (1 << REP_AbilityCDO))
    {
        Ar << AbilityCDO;
    }
    if (RepBits & (1 << REP_SourceObject))
    {
        Ar << SourceObject;
    }
    if (RepBits & (1 << REP_Actors))
    {
        SafeNetSerializeTArray_Default<31>(Ar, Actors);
    }
    if (RepBits & (1 << REP_HitResult))
    {
        if (Ar.IsLoading())
        {
            if (!HitResult.IsValid())
            {
                HitResult = TSharedPtr<FHitResult>(new FHitResult());
            }
        }
        HitResult->NetSerialize(Ar, Map, bOutSuccess);
    }
    if (RepBits & (1 << REP_WorldOrigin))
    {
        Ar << WorldOrigin;
        bHasWorldOrigin = true;
    }
    else
    {
        bHasWorldOrigin = false;
    }
    if (RepBits & (1 << REP_IsCriticalHit))
    {
        Ar << bIsCriticalHit;
    }
 
    if (Ar.IsLoading())
    {
        AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
    }   
     
    bOutSuccess = true;
    return true;
}
