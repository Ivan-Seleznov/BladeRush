// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UQuickBarComponent;
class UCameraComponent;
class UWidgetAnimation;
class UAttributeStamina;
class UInputMappingContext;
class UInputAction;
class UShooterHeroComponent;

USTRUCT(BlueprintType)
struct FHandSwayData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float SideMove;

	UPROPERTY(BlueprintReadOnly)
	float MouseX;
	UPROPERTY(BlueprintReadOnly)
	float MouseY;
};

UCLASS()
class BLADERUSH_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual FVector GetStartGrapplingHookLocation() const override;
	virtual FVector GetGrapplingHookForwardVector() const override;

	void SetMouseAxisValue(const float& X, const float Y);
	void SetMoveXValue(const float& X);
	
	const FHandSwayData& GetHandSwayData() const {return HandSwayData;}
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UShooterHeroComponent> HeroComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FHandSwayData HandSwayData;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons|Preview", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponPreviewMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons|Preview", meta = (AllowPrivateAccess = "true"))
	bool bDestroyPreviewMeshOnSpawn = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons|Preview", meta = (AllowPrivateAccess = "true"))
	FName AttachSocketName = "weapon_socket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons|Preview", meta = (AllowPrivateAccess = "true"))
	FName CameraAttachSocketName = "spine_05";
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UQuickBarComponent> QuickBarComponent;
};
