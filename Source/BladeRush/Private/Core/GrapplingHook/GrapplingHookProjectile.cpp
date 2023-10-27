// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GrapplingHook/GrapplingHookProjectile.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attributes/MovementAttributeSet.h"


AGrapplingHookProjectile::AGrapplingHookProjectile()
{
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision Component"));
	RootComponent = SphereCollisionComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetupAttachment(SphereCollisionComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereCollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 1000;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	
	SphereCollisionComponent->OnComponentHit.AddDynamic(this,&ThisClass::OnProjectileHit);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGrapplingHookProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
		if (Character && FVector::Distance(Character->GetActorLocation(),GetActorLocation()) >= MovementAttributeSet->GetGrapplingProjectileMaxDistance())
		{
			Destroy();
		}
	}
}
void AGrapplingHookProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetInstigator());
	if (!Character) return;
		
	UShooterMovementComponent* ShooterMovementComponent = Character->GetShooterMovementComponent();
	if (!ShooterMovementComponent) return;

	if (HasAuthority() || Character->IsLocallyControlled())
	{
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::White,FString::Printf(TEXT("Point: %s , Normal: %s"),*Hit.ImpactPoint.ToString(),*Hit.ImpactNormal.ToString()));
		ShooterMovementComponent->StartGrappling(FGrapplingHookAttachData(Hit.ImpactPoint,Hit.ImpactNormal));
	}
}



void AGrapplingHookProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileStartLocation = GetActorLocation();
	const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (Character)
	{
		MovementAttributeSet = UMovementAttributeSet::Find(Character->GetAbilitySystemComponent());
	}

}

void AGrapplingHookProjectile::Destroyed()
{
	Super::Destroyed();
	OnProjectileDestroyed.Broadcast(GetOwner());
}
