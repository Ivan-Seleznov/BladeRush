// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GameplayAbilityBase.h"

#include "Characters/BaseCharacter.h"

ABaseCharacter* UGameplayAbilityBase::GetCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}
