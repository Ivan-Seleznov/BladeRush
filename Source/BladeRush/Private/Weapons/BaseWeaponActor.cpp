// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeaponActor.h"
#include "Weapons/BaseWeaponActor.h"
#include "UI/HitMarkerWidgetBase.h"
#include "Characters/BaseCharacter.h"
#include "Data/DecalDataAsset.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Components/PlayerHealthComponent.h"
#include "Weapons/WeaponItemInstance.h"

ABaseWeaponActor::ABaseWeaponActor()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = WeaponMesh;
	
	AimOffsetSceneComponent = CreateDefaultSubobject<USceneComponent>("Aim offset");
	AimOffsetSceneComponent->SetupAttachment(RootComponent);
}

FVector ABaseWeaponActor::GetMuzzleLocation() const
{
	check(WeaponMesh);

	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FRotator ABaseWeaponActor::GetMuzzleRotation() const
{
	check(WeaponMesh);

	return WeaponMesh->GetSocketRotation(MuzzleSocketName);
}

FTransform ABaseWeaponActor::GetHandSocketTransform() const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !Character->GetMesh()) return FTransform::Identity;
	
	const FTransform LeftIKTransform = WeaponMesh->GetSocketTransform(LeftHandIKSocketName);

	FVector OutLoc;
	FRotator OutRot;
	Character->GetMesh()->TransformToBoneSpace(RightHandSocketName,LeftIKTransform.GetLocation(),LeftIKTransform.GetRotation().Rotator(),OutLoc,OutRot);

	FTransform Result;
	Result.SetLocation(OutLoc);
	Result.SetRotation(OutRot.Quaternion());
	Result.SetScale3D(FVector(1,1,1));
	
	return Result;
}

FTransform ABaseWeaponActor::GetAimOffset() const
{
	return AimOffsetSceneComponent->GetRelativeTransform();
}


void ABaseWeaponActor::OnStartReloading(UWeaponItemInstance* WeaponInstance)
{
}

void ABaseWeaponActor::OnFinishReloading(UWeaponItemInstance* WeaponInstance)
{
}

void ABaseWeaponActor::OnClientHit(UWeaponItemInstance* WeaponInstance, TArray<FHitResult> HitResults)
{
	PlayWeaponAnimMontage(WeaponFireMontage);
	PlayCharacterAnimMontage(CharacterFireMontage);
	
	if (WeaponInstance->IsInADS())
	{
		PlayCameraShake(ADSFireCameraShake);
	}
	else
	{
		PlayCameraShake(FireCameraShake);
	}
	
	HandleHits(WeaponInstance,HitResults);
	
	K2_ClientHit(WeaponInstance);
}

void ABaseWeaponActor::OnServerHit(UWeaponItemInstance* WeaponInstance,const TArray<FHitResult>& HitResults)
{
	if (HasAuthority())
	{
		OnHit_Multicast(WeaponInstance,HitResults);
	}
}

void ABaseWeaponActor::OnEnterADS(UWeaponItemInstance* WeaponInstance)
{
	K2_OnEnterADS(WeaponInstance);
}

void ABaseWeaponActor::OnExitADS(UWeaponItemInstance* WeaponInstance)
{
	K2_OnExitADS(WeaponInstance);
}


void ABaseWeaponActor::OnHit_Multicast_Implementation(UWeaponItemInstance* WeaponInstance,
                                                      const TArray<FHitResult>& HitResults)
{
	if (!WeaponInstance) return;

	const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (!Character) return;
	
	if (Character->IsLocallyControlled())
	{
		return;
	}

	PlayWeaponAnimMontage(WeaponFireMontage);
	PlayCharacterAnimMontage(CharacterFireMontage);
	
	WeaponInstance->AddRecoil();
	
	HandleHits(WeaponInstance, HitResults);
}

void ABaseWeaponActor::Destroyed()
{
	Super::Destroyed();

	const ACharacter* Character = GetCharacterOwner<ACharacter>();
	if (Character && Character->IsLocallyControlled() && HitMarkerWidget)
	{
		HitMarkerWidget->RemoveFromParent();
	}
}

void ABaseWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = GetCharacterOwner<ACharacter>();
	if (!Character)
	{
		return;
	}

	if (Character->IsLocallyControlled() && HitMarkerWidgetClass)
	{
		HitMarkerWidget = CreateWidget<UHitMarkerWidgetBase>(GetWorld(),HitMarkerWidgetClass);
		HitMarkerWidget->AddToViewport();
	}
}

ACharacter* ABaseWeaponActor::GetCharacterOwner() const
{
	return Cast<ABaseCharacter>(GetOwner());
}

void ABaseWeaponActor::HandleHits(UWeaponItemInstance* WeaponInstance, const TArray<FHitResult>& HitResults)
{
	for (const FHitResult& Hit : HitResults)
	{
		HandleHit(WeaponInstance,Hit);
	}

	if (ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),ShootSound,GetActorLocation());
	}
	if (MuzzleFlashParticles)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticles,GetWeaponMesh(),MuzzleSocketName,MuzzleFlashOffset.GetLocation(),MuzzleFlashOffset.Rotator(),MuzzleFlashOffset.GetScale3D());
	}
}


void ABaseWeaponActor::HandleHit(UWeaponItemInstance* WeaponInstance, const FHitResult& Hit)
{
	if (bDrawDebugHits && HasAuthority())
	{
		DrawDebugPoint(GetWorld(),Hit.bBlockingHit ? Hit.Location : Hit.TraceEnd,5.f,FColor::Green,false,3.f);
	}

	SpawnDecal(Hit);
	if (TracerParticles)
	{
		UNiagaraComponent* TracerNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),TracerParticles,GetMuzzleLocation() + TracerParticlesOffset.GetLocation(),TracerParticlesOffset.Rotator(),TracerParticlesOffset.GetScale3D());
		TracerNiagaraComp->SetVectorParameter(TracerParticlesEndPointParam,Hit.IsValidBlockingHit() ? Hit.Location : Hit.TraceEnd);
	}
	if (Hit.bBlockingHit)
	{
		if (HitParticles)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(Hit.Location);
			SpawnTransform.SetScale3D(FVector(1,1,1));
			SpawnTransform.SetRotation(Hit.Location.Rotation().Quaternion());
		
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),HitParticles,SpawnTransform);
		}

		const ACharacter* CharacterOwner = GetCharacterOwner();
		if (CharacterOwner && CharacterOwner->IsLocallyControlled())
		{
			if (const ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(Hit.GetActor()))
			{
				if (const UPlayerHealthComponent* HealthComponent = HitCharacter->GetPlayerHealthComponent())
				{
					if (!HealthComponent->IsCharacterDead())
					{
						AddHitMarker();
					}
				}
			}
		}


		K2_HandleHit(Hit);
	}
}

void ABaseWeaponActor::SpawnDecal(const FHitResult& Hit)
{
	if (!DecalData || !Hit.bBlockingHit) return;
	
	UMaterialInstance* DecalMaterial = nullptr;
	if (Hit.PhysMaterial.IsValid())
	{
		DecalMaterial = DecalData->GetMaterialFromPhysicalSurface(Hit.PhysMaterial->SurfaceType);
	}
	else
	{
		DecalMaterial = DecalData->GetDefaultDecalMaterial();
	}

	if (DecalMaterial)
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(),DecalMaterial,DecalSize,Hit.Location,Hit.Location.Rotation(),DecalLifeTime);
	}
}


void ABaseWeaponActor::PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass)
{
	const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (!Character) return;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController) return;

	PlayerController->ClientStartCameraShake(CameraShakeClass);
}

void ABaseWeaponActor::PlayWeaponAnimMontage(UAnimMontage* Montage) const
{
	if (!Montage || !WeaponMesh) return;
	
	UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->Montage_Play(Montage);
}

void ABaseWeaponActor::PlayCharacterAnimMontage(UAnimMontage* Montage) const
{
	if (!Montage) return;
	
	const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (!Character || !Character->GetMesh()) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->Montage_Play(Montage);
}

void ABaseWeaponActor::AddHitMarker()
{
	if (HitMarkerWidget)
	{
		HitMarkerWidget->AddHitMarker();
		if (HitMarkerSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(),HitMarkerSound,HitMarkerSoundVolumeMultiplier);
		}
	}
}
