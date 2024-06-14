// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BladeRushGlobals.h"
#include "ShooterMovementComponent.generated.h"

#define ERROR_VALUE -1.f;

class UMovementAttributeSet;
class UCableComponent;
class ABaseCharacter;
class AGrapplingHookProjectile;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrappleExit, ABaseCharacter* /*Character*/);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMovementActionStartedDelegate, TEnumAsByte<EMovementAction> ,CurrentMovementAction, TEnumAsByte<EMovementAction>, PrevMovementAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementActionEndedDelegate, TEnumAsByte<EMovementAction>, EndedMovementAction);

UENUM(BlueprintType)
enum class ETransitionName  : uint8
{
	None,
	Mantle
};

UENUM(BlueprintType)
enum class EMantleType : uint8
{
	Tall,
	Short
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
		uint8 Saved_bWantsToMantle:1;
        uint8 Saved_bWantsToDash:1;
	    
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
	/*Safe variables & fags*/
	/*Client flag*/
	bool Safe_bWantsToSprint;
	bool Safe_bWantsToGrapple;
	bool Safe_bWantsToMantle;
    bool Safe_bWantsToDash;

	/*Other variables*/
	bool Safe_bWantsToSlide;
	bool Safe_bWallRunIsRight;
	
	UShooterMovementComponent();

	FMovementActionStartedDelegate MovementActionStartedDelegate;
	FMovementActionEndedDelegate MovementActionEndedDelegate;
	
	FOnGrappleExit OnGrappleExit;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	virtual float GetCustomMaxSpeed() const;
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;
	
	virtual bool IsMovingOnGround() const override;

	virtual bool CanCrouchInCurrentState() const override;
	
	UFUNCTION(BlueprintPure)
	TEnumAsByte<EMovementAction> GetCurrentMovementAction() const {return CurrentMovementAction;}

    UFUNCTION(BlueprintCallable)
    void Dash();

    UFUNCTION(BlueprintPure)
    bool IsCharacterOwnerLocallyControlled() const;
    
	UFUNCTION(BlueprintCallable)
	bool TryGrapple();
	void StartGrappling(const FGrapplingHookAttachData& AttachData);
	void StopGrappling();
	
	UFUNCTION(BlueprintPure)
	bool CanGrapple() const;
	
	UFUNCTION(BlueprintPure)
	bool CanSprint() const;

	UFUNCTION(BlueprintPure)
	bool CanSlide() const;

	UFUNCTION(BlueprintPure)
	bool CanMantle() const;

	UFUNCTION(BlueprintPure)
	bool CanDash() const;
	
	UFUNCTION(BlueprintPure)
	bool CanUpdateCharacterRotation() const;
	
	UFUNCTION(BlueprintPure)
	bool IsMovementMode(EMovementMode InMovementMode) const;

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	UFUNCTION(BlueprintPure)
	bool IsInMantle() const {return bInMantle;}
	
	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const {return IsCustomMovementMode(CMOVE_WallRun);} 

	UFUNCTION(BlueprintPure)
	bool IsGrappling() const {return IsCustomMovementMode(CMOVE_Grappling);} 

	UFUNCTION(BlueprintPure)
	bool IsSliding() const {return IsCustomMovementMode(CMOVE_Slide);} 
	
	UFUNCTION(BlueprintPure)
	bool IsWallRunningRight() const {return Safe_bWallRunIsRight;}
	
	UFUNCTION(BlueprintPure)
	bool IsMoving() const;

	UFUNCTION(BlueprintPure)
	bool IsRotatingAroundYaw() const;

	UFUNCTION(BlueprintPure)
	bool IsTurningRight() const {return bCharacterRotatingRight;}

	UFUNCTION(BlueprintPure)
	bool IsTurningLeft() const {return bCharacterRotatingLeft;}

	UFUNCTION(BlueprintCallable)
	void ResetAllMovementActions();

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	void ExitCustomMovementMode(uint8 PreviousCustomMode);
	void EnterCustomMovementMode();
	
	virtual void UpdateCharacterRotation(float DeltaTime);

    void EnterDash();
    virtual void PhysFlying(float deltaTime, int32 Iterations) override;
    
	void StartNewMovementAction(EMovementAction NewMovementAction);
    void ExitDash();
	void EndCurrentMovementAction();
	
	/*Helpers*/
	ACharacter* GetDefaultCharacter() const;

    /*Dash*/
    UPROPERTY(BlueprintReadOnly,Category="Dash")
    float DashStartTime;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Dash")
    float DashImpulse = 7200.f;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Dash")
    float DashFallingImpulse = 2000.f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Dash")
    float BaseDashFlyingTime = 2.f;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Dash")
    float DashFlyingTime = 2.f;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Dash")
    float FallingDashFlyingTimeMultiplier = 2.f;
    
    /*Sprint*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Replicated)
	float MaxSpeedMultiplier = 1.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Sprint")
	float MaxSprintSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly,Category="Sprint|Crouching")
	float MaxCrouchingSprintSpeed = 350.f;
	
	/*Mantle*/
	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float MantleMaxWallHeight = 300.f;
	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float MantleMaxDistance = 200.f;

	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float MantleMaxSurfaceAngle = 45.f;

	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float MantleMaxWallSteepnessAngle = 120.f;
	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float MantleMinWallSteepnessAngle = 75.f;
	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float MantleMaxAlignmentAngle = 45.f;

	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	int32 MantleRayCount = 6;

	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	FMantleAnimData ShortMantleAnimData;
	UPROPERTY(EditDefaultsOnly,Category="Mantle") 
	FMantleAnimData TallMantleAnimData;

	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float ShortMantleTransitionZOffset = 0.0f;
	UPROPERTY(EditDefaultsOnly,Category="Mantle")
	float TallMantleTransitionZOffset = 0.0f;

	/*Slide*/
	UPROPERTY(EditDefaultsOnly,Category="Slide")
	float MinSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide")
	float MaxSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide")
	float SlideEnterImpulse=400.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide")
	float SlideGravityForce=4000.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide")
	float SlideFrictionFactor = 0.06f;
	UPROPERTY(EditDefaultsOnly,Category="Slide")
	float BrakingDecelerationSliding=1000.f;
	UPROPERTY(EditDefaultsOnly,Category="Slide")
	float SlideAccelerationFactor = 0.45f;
	
	/*WallRun*/
	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float MinWallRunSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float MaxWallRunSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float MaxVerticalWallRunSpeed = 180.f; //?
	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float MinWallRunHeight = 75.f;
	
	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float WallJumpOfForce = 400.f; //Force with which a player jumps away from a wall

	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float WallRunPullAwayAngle = 75.f;
	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float WallAttractionForce = 200.f;

	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	float WallRunEnteredAngle = 0.f;
	
	UPROPERTY(EditDefaultsOnly,Category="WallRun")
	UCurveFloat* WallRunGravityScaleCurve;
	
	/*Grappling hook*/
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float GrapplingHookDistance = 2000.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float MaxGrapplingSpeed = 1090.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float GrapplingBrakingDeceleration = 2000.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float GrapplingGravityScale = 0.1f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float GrapplingReleasedDistance = 150.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float GrapplingSideAccelerationFactor = 1.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float GrapplingVerticalJumpBoost = 295.f;
	UPROPERTY(EditDefaultsOnly,Category="Grappling")
	float GrapplingHorizontalJumpBoost = 1.f;
	
	UPROPERTY(Transient)
	ABaseCharacter* ShooterCharacterOwner;

private:
    /*Dash*/
    void PerformDash();
    
	/*Slide*/
	void EnterSlide();
	void ExitSlide();

	void PhysSlide(float DeltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

	bool IsWallOnSideTrace(FHitResult& WallHit, bool bWallRight) const;
	
	/*Mantle*/
	void EnterMantle();
	void ExitMantle();
	
	bool TryMantle(); // should be called on the client
	
	FVector GetMantleStartLocation(const FHitResult& FrontHit, const FHitResult& SurfaceHit, EMantleType MantleType) const;
	float GetCapsuleRadius() const;
	float GetCapsuleHalfHeight() const;

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_PlayMantleProxyAnim(UAnimMontage* ProxyMontage);
	
	/*WallRun*/
	void EnterWallRun();
	void ExitWallRun();
	
	bool TryWallRun();
	bool CanEnterWallRun(const FHitResult& WallHit);
	void PhysWallRun(float DeltaTime, int32 Iterations);

	/*Grappling hook*/
	void EnterGrapple();
	void ExitGrapple();

	void PhysGrappling(float DeltaTime,int32 Iterations);
	
	AGrapplingHookProjectile* SpawnGrapplingHookProjectile(const FVector& Location, const FVector& Direction);
	void EnableGrapplingHookCableComponent(bool bEnabled);
	
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_TryGrapple(const FVector& ProjectileDirection,UCableComponent* CableComponent);
	
	UFUNCTION(Server,Reliable)
	void Server_TryGrapple(const FVector& ProjectileDirection);

	UFUNCTION(Server,Reliable)
	void Server_SetGrapplingHookAttachPoint(const FGrapplingHookAttachData& AttachData);
    
	UFUNCTION(Unreliable,Server)
	void Server_ExitGrapple();
    	
	UFUNCTION(Unreliable,NetMulticast)
	void Multicast_ExitGrapple();
	
	void SetDefaultRotationData();
	
	
	bool Safe_bTransitionFinished;
	
	bool Safe_bHadAnimRootMotion;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	UPROPERTY(Transient)
	UAnimMontage* TransitionQueuedMontage;
	float TransitionQueuedMontageSpeed;
	int TransitionRMS_ID;
	ETransitionName TransitionName;

	
	UFUNCTION()
	void OnRep_bInMantle(bool OldbInMantle);
	UPROPERTY(Replicated,ReplicatedUsing=OnRep_bInMantle)
	bool bInMantle;
	
	FGrapplingHookAttachData GrapplingHookAttachData;
	UPROPERTY(EditDefaultsOnly, Category = "Grappling|Projectile")
	TSubclassOf<AGrapplingHookProjectile> ProjectileClass;
	UPROPERTY()
	AGrapplingHookProjectile* GrapplingHookProjectile;

	UPROPERTY()
	const UMovementAttributeSet* MovementAttributeSet;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float RotationThreshold = 10.f;
	float PrevRotationYaw = 0.f;
	float RotationTimeElapsed = 0.f;
	
	float StartRotationYaw;
	bool bEndRotating;
	
	bool bCharacterRotatingLeft = false;
	bool bCharacterRotatingRight = false;

	bool bPrevWantsToMantle = false;
	
	UPROPERTY()
	UAnimMontage* CurrentProxyMontage;

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EMovementAction> CurrentMovementAction = EMovementAction::None;
};
