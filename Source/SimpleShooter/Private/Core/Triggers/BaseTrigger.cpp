// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Triggers/BaseTrigger.h"
#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"

ABaseTrigger::ABaseTrigger()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
}


