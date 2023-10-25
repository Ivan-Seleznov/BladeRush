// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterMovementComponent.generated.h"

#define ERROR_VALUE -1.f;

class UMovementAttributeSet;
DECLARE_MULTICAST_DELEGATE_OneParam(FGrappleFailed, AActor* /*Owner*/)

class UCableComponent;
class ABaseCharacter;
class AGrapplingHookProjectile;


UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Slide		UMETA(DisplayName = "Slide"),
	CMOVE_WallRun	UMETA(DisplayName="WallRun"),
	CMOVE_Grappling UMETA(DisplayName="Grappling"),
	CMOVE_Max		UMETA(Hidden)
};

UENUM(BlueprintType)
enum ETransitionName
{
	TNAME_None		UMETA(DispayName="NoneTransition"),
	TNAME_Mantle	UMETA(DispayName="MantleTransition"),
};

UENUM(BlueprintType)
enum EMantleType
{
	TMANTLE_Tall	UMETA(DispayName="TallMantle"),
	TMANTLE_Short	UMETA(DispayName="ShortMantle"),
};

USTRUCT()
struct FMantleAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MantleMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TransitionMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ProxyMontage;
};

USTRUCT()
struct FGrapplingHookAttachData
{
	GENERATED_BODY()

	FGrapplingHookAttachData();
	
	FGrapplingHookAttachData(FVector Point, FVector Normal);
	
	UPROPERTY(EditDefaultsOnly)
	FVector_NetQuantize AttachPoint;

	UPROPERTY(EditDefaultsOnly)
	FVector_NetQuantizeNormal SurfaceNormal;
};

/**
 * Custom shooter movement component
 */
UCLASS()
class BLADERUSH_API UShooterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Shooter : public FSavedMove_Character
	{
		/*SavedMove Flags*/
		uint8 Saved_bWantsToSprint:1;
		uint8 Saved_bPressedPlayerJump:1;
		uint8 Saved_bWantsToGrapple:1;

		/*SavedMove variable*/
		uint8 Saved_bWantsToSlide:1;
		uint8 Saved_bHadAnimRootMotion:1;
		uint8 Saved_bTransitionFinished:1;
		uint8 Saved_bWallRunIsRight:1;
		
		/*Check can we combine two moves*/
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		
		virtual uint8 GetCompressedFlags() const override;

		/*Capture the state data of cmc*/
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;

		virtual void Clear() override;
	};

	class FNetworkPredictionDataClient_Shooter : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionDataClient_Shooter(const UCharacterMovementComponent& ClientMovement);
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
public:
	
	UShooterMovementComponent();
	virtual void InitializeComponent() override;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	/*Client flag*/
	bool Safe_bWantsToSprint;
	bool Safe_bWantsToGrapple;
	
	/*Other variables*/
	bool Safe_bWantsToSlide;
	
	UFUNCTION(BlueprintCallable)
	bool TryGrapple();
	
	UFUNCTION(BlueprintPure)
	bool IsMovementMode(EMovementMode InMovementMode) const;

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;
	
	bool CanSprint() const;
	bool CanGrapple() const;
	
	void StartGrappling(const FGrapplingHookAttachData& AttachData);
	void StopGrappling();

	FGrappleFailed OnGrappleFailed;
	
	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const {return IsCustomMovementMode(CMOVE_WallRun);} 

	UFUNCTION(BlueprintPure)
	bool IsGrappling() const {return IsCustomMovementMode(CMOVE_Grappling);} 
	
	UFUNCTION(BlueprintPure)
	bool IsWallRunningRight() const {return Safe_bWallRunIsRight;}
	
	/*Sprint*/
	UPROPERTY(EditDefaultsOnly,Category="Sprint")
	float MaxSprintSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly,Category="Sprint|Crouching")
	float MaxCrouchingSprintSpeed = 350.f;

	bool CanSlide() const;
	
	UFUNCTION()
	FVector_NetQuantize GetMoveVector() const {return MoveVector;}
	
	UFUNCTION(Client, Unreliable)
	void Client_SetMoveVector(const FVector2D& NewValue);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
protected:
	/*Mantle*/
	UPROPERTY(EditDefaultsOnly,Category="Mantle") float MantleMaxWallHeight = 300.f;
	UPROPERTY(EditDefaultsOnly,Category="Mantle") float MantleMaxDistance = 200.f;

	UPROPERTY(EditDefaultsOnly,Category="Mantle") float MantleMaxSurfaceAngle = 45.f;

	UPROPERTY(EditDefaultsOnly,Category="Mantle") float MantleMaxWallSteepnessAngle = 120.f;
	UPROPERTY(EditDefaultsOnly,Category="Mantle") float MantleMinWallSteepnessAngle = 75.f;
	UPROPERTY(EditDefaultsOnly,Category="Mantle") float MantleMaxAlignmentAngle = 45.f;

	UPROPERTY(EditDefaultsOnly,Category="Mantle") int MantleRayCount = 6;

	UPROPERTY(EditDefaultsOnly,Category="Mantle") FMantleAnimData ShortMantleAnimData;
	UPROPERTY(EditDefaultsOnly,Category="Mantle") FMantleAnimData TallMantleAnimData;

	/*Slide*/
	UPROPERTY(EditDefaultsOnly,Category="Slide") float MinSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide") float MaxSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide") float SlideEnterImpulse=400.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide") float SlideGravityForce=4000.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide") float SlideFrictionFactor=.06f;
	UPROPERTY(EditDefaultsOnly,Category="Slide") float BrakingDecelerationSliding=1000.f;

	/*WallRun*/
	UPROPERTY(EditDefaultsOnly,Category="WallRun") float MinWallRunSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly,Category="WallRun") float MaxWallRunSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly,Category="WallRun") float MaxVerticalWallRunSpeed = 180.f; //?
	UPROPERTY(EditDefaultsOnly,Category="WallRun") float MinWallRunHeight = 75.f;
	
	UPROPERTY(EditDefaultsOnly,Category="WallRun") float WallJumpOfForce = 400.f; //Force with which a player jumps away from a wall

	UPROPERTY(EditDefaultsOnly,Category="WallRun") float WallRunPullAwayAngle = 75.f;
	UPROPERTY(EditDefaultsOnly,Category="WallRun") float WallAttractionForce = 200.f;

	UPROPERTY(EditDefaultsOnly,Category="WallRun") UCurveFloat* WallRunGravityScaleCurve;

	
	/*Grappling hook*/
	UPROPERTY(EditDefaultsOnly,Category="Grappling") float GrapplingHookDistance = 2000.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling") float MaxGrapplingSpeed = 1090.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling") float GrapplingBrakingDeceleration = 2000.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling") float GrapplingGravityScale = 0.1f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling") float GrapplingReleasedDistance = 150.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Grappling") float GrapplingVerticalJumpBoost = 295.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling") float GrapplingHorizontalJumpBoost = 1.f;
	
	UPROPERTY(Transient) ABaseCharacter* ShooterCharacterOwner;

	UPROPERTY(Replicated)
	FVector_NetQuantize MoveVector;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	ACharacter* GetDefaultCharacter() const;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
	UFUNCTION(Server, Unreliable)
	void Server_SetMoveVector(const FVector2D& NewValue);

	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void BeginPlay() override;
private:
	void EnterSlide();
	void ExitSlide();

	void PhysSlide(float DeltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

	bool IsWallOnSideTrace(FHitResult& WallHit, bool bWallRight) const;

	/*Call on client*/
	bool TryMantle();
	
	FVector GetMantleStartLocation(const FHitResult& FrontHit, const FHitResult& SurfaceHit, EMantleType MantleType) const;
	float GetCapsuleRadius() const;
	float GetCapsuleHalfHeight() const;

	bool TryWallRun();
	void PhysWallRun(float DeltaTime, int32 Iterations);
	
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_TryGrapple(AGrapplingHookProjectile* Projectile,UCableComponent* CableComponent);
	
	UFUNCTION(Server,Reliable)
	void Server_TryGrapple(const FVector& ProjectileDirection);
	
	void ExitGrapple();
	
	bool Safe_bTransitionFinished;
	
	bool Safe_bHadAnimRootMotion;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	UPROPERTY(Transient) UAnimMontage* TransitionQueuedMontage;
	float TransitionQueuedMontageSpeed;
	int TransitionRMS_ID;
	ETransitionName TransitionName;

	bool Safe_bWallRunIsRight;

	void PhysGrappling(float DeltaTime,int32 Iterations);
	
	//FHitResult AttachPointHit;
	
	FGrapplingHookAttachData GrapplingHookAttachData;

	UPROPERTY(EditDefaultsOnly, Category = "Grappling|Projectile")
	TSubclassOf<AGrapplingHookProjectile> ProjectileClass;
	
	UPROPERTY()
	AGrapplingHookProjectile* GrapplingHookProjectile;

	UPROPERTY() const UMovementAttributeSet* MovementAttributeSet;
	
	UFUNCTION()
	void OnGrapplingHookProjectileDestroyed(AActor* ProjectileOwner);

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_ExitGrapple(ABaseCharacter* Character);
	
	//UPROPERTY(EditDefaultsOnly, Category = "Grappling|Cable")
	//TSubclassOf<ACableActor> GrapplingCableClass;
};
