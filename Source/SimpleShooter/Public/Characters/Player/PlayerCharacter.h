// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

class UCameraComponent;
class UWidgetAnimation;
class UAttributeStamina;
class UInputMappingContext;
class UInputAction;
class UShooterHeroComponent;
class USpringArmComponent;

UCLASS()
class SIMPLESHOOTER_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties

	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UShooterHeroComponent> HeroComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
};
