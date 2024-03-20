// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Execution/HitDamageExecution.h"

#include "BladeRushAbilityTypes.h"
#include "GAS/Attributes/AttributeHitPoints.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(HitDamage);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeHitPoints, HitDamage, Target, true);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics Stat;
	return Stat;
}


UHitDamageExecution::UHitDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().HitDamageDef);
}

bool UHitDamageExecution::IsCriticalDamage(float BaseCritChance) const
{
	return FMath::FRand() < BaseCritChance;
}

void UHitDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                 FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC->GetAvatarActor();
	if (!TargetActor) return;
	
	/*UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceASC->GetAvatarActor();
	if (!SourceActor) return;*/
	
	FGameplayEffectSpec* Spec = ExecutionParams.GetOwningSpecForPreExecuteMod();
	if (!Spec) return;

	FShooterGameplayEffectContext* EffectContext = static_cast<FShooterGameplayEffectContext*>(Spec->GetContext().Get());
	if (!EffectContext) return;
	
	const FGameplayTagContainer* SourceTags = Spec->CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec->CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	const float BaseDamage = FMath::Max<float>(Spec->GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Effect.Damage")),false,-1.f),0.f);
	const float CritChance = FMath::Max<float>(Spec->GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Chance.Crit")),false,-1.f),0.f);
	
	// calculations here
	if (IsCriticalDamage(CritChance))
	{
		EffectContext->SetIsCriticalHit(true);
	}
	
	const float FinalDamage = BaseDamage;
	
	if (FinalDamage > 0)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HitDamageProperty,EGameplayModOp::Additive,FinalDamage));
	}

	//may be add receive damage event in ASC
}
