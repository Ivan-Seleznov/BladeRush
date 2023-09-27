// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Triggers/GameplayEffectTrigger.h"
#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"

AGameplayEffectTrigger::AGameplayEffectTrigger()
{
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGameplayEffectTrigger::OnOverlapBegin);
}

void AGameplayEffectTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		if (GameplayEffect)
		{
			FGameplayEffectContextHandle EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(GameplayEffect.GetDefaultObject(),1.f,EffectContext);
			UE_LOG(LogTemp,Warning,TEXT("ApplyGameplayEffect [%s] to [%s]"),*GameplayEffect->GetName(),*OtherActor->GetName());
		}
		if (bEnableTeleport)
		{
			if (Character->HasAuthority())
			{
				Character->SetActorLocation(NewActorLocation);
			}
		}
	}
}
