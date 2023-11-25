// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentInstance.h"

#include "Characters/BaseCharacter.h"
#include "Equipment/EquipmentDefinition.h"
#include "Net/UnrealNetwork.h"

UEquipmentInstance::UEquipmentInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

UWorld* UEquipmentInstance::GetWorld() const
{
	return GetPawn() ? GetPawn()->GetWorld() : nullptr;
}

APawn* UEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void UEquipmentInstance::SpawnEquipmentActors(const TArray<FEquipmentActorToSpawn>& ActorsToSpawn)
{
	ACharacter* Character = Cast<ACharacter>(GetPawn());
	if (!Character) return;

	for (const FEquipmentActorToSpawn& EquipmentActorToSpawn : ActorsToSpawn)
	{
		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(EquipmentActorToSpawn.ActorToSpawn,FTransform::Identity,Character);
		NewActor->FinishSpawning(FTransform::Identity,true);
		NewActor->SetActorRelativeTransform(EquipmentActorToSpawn.AttachTransform);
		NewActor->AttachToComponent(Character->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,EquipmentActorToSpawn.AttachSocket);

		SpawnedActors.Add(NewActor);
	}
}

void UEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UEquipmentInstance::OnEquipped()
{
}

void UEquipmentInstance::OnUnequipped()
{
}
