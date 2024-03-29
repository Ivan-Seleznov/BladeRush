// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ShooterMovementComponent.h"

#include "BladeRushLogs.h"
#include "BlendSpaceAnalysis.h"
#include "CableComponent.h"
#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Core/GrapplingHook/GrapplingHookProjectile.h"
#include "GameFramework/Character.h"
#include "GAS/Attributes/MovementAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

UShooterMovementComponent::UShooterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
	NavAgentProps.bCanJump = true;
	NavAgentProps.bCanWalk = true;
	bOrientRotationToMovement = false;
}

void UShooterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	ShooterCharacterOwner = Cast<ABaseCharacter>(GetOwner());

	MovementAttributeSet = UMovementAttributeSet::Find(ShooterCharacterOwner->GetAbilitySystemComponent());
}
void UShooterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,bInMantle);
}


bool UShooterMovementComponent::CanSprint() const
{
	return IsMovementMode(MOVE_Walking) && (Velocity.GetSafeNormal2D() | UpdatedComponent->GetForwardVector().GetSafeNormal()) > 0.1f && Velocity.SizeSquared() > 4;
}

bool UShooterMovementComponent::CanGrapple() const
{
	return true; /*Add condition and movement modes in which player can not grapple. Example - !IsCustomMovementMode(CMOVE_WallRun)*/
}


#pragma region CMC
FGrapplingHookAttachData::FGrapplingHookAttachData()
{
}

FGrapplingHookAttachData::FGrapplingHookAttachData(FVector Point, FVector Normal) : AttachPoint(Point), SurfaceNormal(Normal){}

bool UShooterMovementComponent::FSavedMove_Shooter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Shooter* NewShooterMove = static_cast<FSavedMove_Shooter*>(NewMove.Get());
	if (Saved_bWantsToSprint != NewShooterMove->Saved_bWantsToSprint)
	{
		return false;
	}
	if (Saved_bWallRunIsRight != NewShooterMove->Saved_bWallRunIsRight)
	{
		return false;
	}
	if (Saved_bWantsToGrapple != NewShooterMove->Saved_bWantsToGrapple)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

uint8 UShooterMovementComponent::FSavedMove_Shooter::GetCompressedFlags() const
{
	/*need client to replicate data*/
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;
	if (Saved_bWantsToGrapple) Result |= FLAG_Custom_1;
	if (Saved_bPressedPlayerJump) Result |= FLAG_JumpPressed;

	return Result;
}

void UShooterMovementComponent::FSavedMove_Shooter::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UShooterMovementComponent* ShooterMovementComponent = Cast<UShooterMovementComponent>(C->GetCharacterMovement());
	
	Saved_bWantsToSprint = ShooterMovementComponent->Safe_bWantsToSprint;
	Saved_bWantsToSlide = ShooterMovementComponent->Safe_bWantsToSlide;
	
	Saved_bPressedPlayerJump = ShooterMovementComponent->ShooterCharacterOwner->bPlayerPressedJump;

	Saved_bHadAnimRootMotion = ShooterMovementComponent->Safe_bHadAnimRootMotion;
	Saved_bTransitionFinished = ShooterMovementComponent->Safe_bTransitionFinished;

	Saved_bWallRunIsRight = ShooterMovementComponent->Safe_bWallRunIsRight;
	
	Saved_bWantsToGrapple = ShooterMovementComponent->Safe_bWantsToGrapple;
}

void UShooterMovementComponent::FSavedMove_Shooter::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UShooterMovementComponent* ShooterMovementComponent = Cast<UShooterMovementComponent>(C->GetCharacterMovement());
	
	ShooterMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
	ShooterMovementComponent->Safe_bWantsToSlide = Saved_bWantsToSlide;
	
	ShooterMovementComponent->ShooterCharacterOwner->bPlayerPressedJump = Saved_bPressedPlayerJump;
	
	ShooterMovementComponent->Safe_bHadAnimRootMotion = Saved_bHadAnimRootMotion;
	ShooterMovementComponent->Safe_bTransitionFinished = Saved_bTransitionFinished;

	ShooterMovementComponent->Safe_bWallRunIsRight = Saved_bWallRunIsRight;
	
	ShooterMovementComponent->Safe_bWantsToGrapple = Saved_bWantsToGrapple;
}

void UShooterMovementComponent::FSavedMove_Shooter::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
	Saved_bPressedPlayerJump = 0;
	
	Saved_bHadAnimRootMotion = 0;
	Saved_bTransitionFinished = 0;
	
	Saved_bWallRunIsRight = 0;
	Saved_bWantsToGrapple = 0;
}

void UShooterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Shooter::FLAG_Custom_0) != 0;
	Safe_bWantsToGrapple = (Flags & FSavedMove_Shooter::FLAG_Custom_1) != 0;
}

UShooterMovementComponent::FNetworkPredictionDataClient_Shooter::FNetworkPredictionDataClient_Shooter(const UCharacterMovementComponent& ClientMovement)
: FNetworkPredictionData_Client_Character(ClientMovement)
{
}

FSavedMovePtr UShooterMovementComponent::FNetworkPredictionDataClient_Shooter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Shooter());
}

FNetworkPredictionData_Client* UShooterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner)

	if (!ClientPredictionData)
	{
		UShooterMovementComponent* MutableThis = const_cast<UShooterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionDataClient_Shooter(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

void UShooterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	
	if (PreviousMovementMode == MOVE_Custom)
	{
		ExitCustomMovementMode(PreviousCustomMode);
	}
	
	if (MovementMode == MOVE_Custom)
	{
		EnterCustomMovementMode();
	}
}

void UShooterMovementComponent::ExitCustomMovementMode(uint8 PreviousCustomMode)
{
	switch (PreviousCustomMode)
	{
	case CMOVE_Slide:
		ExitSlide();
		break;
			
	case CMOVE_Grappling:
		ExitGrapple();
		break;
			
	case CMOVE_WallRun:
		ExitWallRun();
		break;
			
	default:
		break;
	}
}

void UShooterMovementComponent::EnterCustomMovementMode()
{
	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		EnterSlide();
		break;
			
	case CMOVE_Grappling:
		EnterGrapple();
		break;
			
	case CMOVE_WallRun:
		EnterWallRun();
		break;
			
	default:
		break;
	}
}

void UShooterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	if (!HasAnimRootMotion() && Safe_bHadAnimRootMotion && IsMovementMode(MOVE_Flying))
	{
		SetMovementMode(MOVE_Walking);
		if (bInMantle)
		{
			bInMantle = false;
			EndCurrentMovementAction();
			
			if (CharacterOwner->HasAuthority())
			{
				DEBUG_SLOG("SERVER MantleFinished",FColor::Orange)
			}
			else
			{
				DEBUG_SLOG("CLIENT MantleFinished",FColor::Yellow)
			}
		}

	}
	if (GetRootMotionSourceByID(TransitionRMS_ID) && GetRootMotionSourceByID(TransitionRMS_ID)->Status.HasFlag(ERootMotionSourceStatusFlags::Finished))
	{
		RemoveRootMotionSourceByID(TransitionRMS_ID);
		Safe_bTransitionFinished = true;
	}

	Safe_bHadAnimRootMotion = HasAnimRootMotion();
}

void UShooterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);

	if (CanUpdateCharacterRotation())
	{
		UpdateCharacterRotation(DeltaSeconds);
	}
	else
	{
		SetDefaultRotationData();
	}
	
	PrevRotationYaw = UpdatedComponent->GetComponentRotation().Yaw;
	
	//WallRun
	if (IsFalling())
	{
		TryWallRun();
	}

	if (ShooterCharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		if (!IsGrappling() && Safe_bWantsToGrapple)
		{
			SetMovementMode(MOVE_Custom,CMOVE_Grappling);
		}

		if (IsGrappling() && !Safe_bWantsToGrapple)
		{
			SetMovementMode(MOVE_Falling);
		}	
	}
	
	if (CanSlide() && IsMovementMode(MOVE_Walking) && bWantsToCrouch && Safe_bWantsToSlide)
	{
		SetMovementMode(MOVE_Custom,CMOVE_Slide);
		Safe_bWantsToSlide = false;
	}
	
	if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch) 
	{
		SetMovementMode(MOVE_Walking);
	}

	if (Safe_bWantsToSprint && !CanSprint())
	{
		Safe_bWantsToSprint = false;
	}
	
	//Mantle
	if (ShooterCharacterOwner->bPlayerPressedJump)
	{
		if (!bInMantle)
		{
			if (TryMantle())
			{
				ShooterCharacterOwner->StopJumping();
				//ShooterCharacterOwner->bUseControllerRotationYaw = false;
				bInMantle = true;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1,15,FColor::Orange,"FAILED MANTLE.. JUMP");
				ShooterCharacterOwner->bPlayerPressedJump = false;
				CharacterOwner->bPressedJump = true;
				/*Perform jump physics*/
				CharacterOwner->CheckJumpInput(DeltaSeconds);
			}
		}
	}

	//WallRun
	if (IsFalling())
	{
		TryWallRun();
	}
	
	if (Safe_bTransitionFinished)
	{
		if (ShooterCharacterOwner->HasAuthority())
		{
			DEBUG_SLOG(TEXT("SERVER Transition Finished"),FColor::Blue);

		}
		else
		{
			DEBUG_SLOG(TEXT("CLIENT Transition Finished"),FColor::Blue);
		}
		if (TransitionName == ETransitionName::Mantle)
		{
			if (IsValid(TransitionQueuedMontage) && TransitionQueuedMontage->HasRootMotion())
			{
				SetMovementMode(MOVE_Flying);
				CharacterOwner->PlayAnimMontage(TransitionQueuedMontage,TransitionQueuedMontageSpeed);
				TransitionQueuedMontageSpeed = 0.f;
				TransitionQueuedMontage = nullptr;
				
				if (CurrentProxyMontage && ShooterCharacterOwner->HasAuthority())
				{
					Multicast_PlayMantleProxyAnim(CurrentProxyMontage);
					CurrentProxyMontage = nullptr;
				}
			}
			else
			{
				SetMovementMode(MOVE_Walking);

				bInMantle = false;
				EndCurrentMovementAction();
			}
			Safe_bTransitionFinished = false;
		}
	}
}

void UShooterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);
	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime,Iterations);
		break;
	case CMOVE_WallRun:
		PhysWallRun(deltaTime,Iterations);
		break;
	case CMOVE_Grappling:
		PhysGrappling(deltaTime,Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal,TEXT("Invalid Movement Mode"));
		break;
	}
}

void UShooterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UShooterMovementComponent::UpdateCharacterRotation(float DeltaTime)
{
	if (!IsRotatingAroundYaw())
	{
		SetDefaultRotationData();
		bEndRotating = true;
		return;
	}


	if (bEndRotating)
	{
		StartRotationYaw = UpdatedComponent->GetComponentRotation().Yaw;
		bEndRotating = false;
	}

	const float DeltaRotation = FMath::Abs(UpdatedComponent->GetComponentRotation().Yaw) - FMath::Abs(StartRotationYaw);
	if (FMath::Abs(DeltaRotation) >= RotationThreshold)
	{
		if (UpdatedComponent->GetComponentRotation().Yaw > PrevRotationYaw)
		{
			bCharacterRotatingLeft = false;
			bCharacterRotatingRight = true;
			GEngine->AddOnScreenDebugMessage(-1,0.f,FColor::Green,"RIGHT");
		}
		else
		{
			bCharacterRotatingRight = false;
			bCharacterRotatingLeft = true;
			GEngine->AddOnScreenDebugMessage(-1,0.f,FColor::Blue,"LEFT");
		}
	}
	else
	{
		SetDefaultRotationData();
	}

	RotationTimeElapsed += DeltaTime;
}

void UShooterMovementComponent::StartNewMovementAction(EMovementAction NewMovementAction)
{
	const EMovementAction PrevMovementAction = CurrentMovementAction;
	CurrentMovementAction = NewMovementAction;
	
	switch (CurrentMovementAction)
	{
	case EMovementAction::Mantle:
		EnterMantle();
		break;
		
	default:
		break;
	}
	
	MovementActionStartedDelegate.Broadcast(CurrentMovementAction,PrevMovementAction);
}

void UShooterMovementComponent::EndCurrentMovementAction()
{
	const EMovementAction EndedMovementAction = CurrentMovementAction;
	CurrentMovementAction = EMovementAction::None;

	MovementActionEndedDelegate.Broadcast(EndedMovementAction);

	switch (EndedMovementAction)
	{
	case EMovementAction::Mantle:
		ExitMantle();
		break;
		
	default:
		break;
	}
}


bool UShooterMovementComponent::CanUpdateCharacterRotation() const
{
	return !CharacterOwner->HasAnyRootMotion() && !IsMoving();
}

bool UShooterMovementComponent::IsMoving() const
{
	return Velocity.Length() > 1.f && Acceleration.Length() > 0;
}

bool UShooterMovementComponent::IsRotatingAroundYaw() const
{
	return !FMath::IsNearlyEqual(PrevRotationYaw,UpdatedComponent->GetComponentRotation().Yaw,0.001f);
}

float UShooterMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking) && Safe_bWantsToSprint)
	{
		if (bWantsToCrouch) return MaxCrouchingSprintSpeed;

		return MaxSprintSpeed;
	}
	
	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxSpeed();
	}

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return MaxSlideSpeed;
	case CMOVE_WallRun:
		return MaxWallRunSpeed;
	case CMOVE_Grappling:
		return MaxGrapplingSpeed;
	default:
		UE_LOG(LogTemp,Fatal,TEXT("Invalid Movement Mode (getmaxspeed)"));
		return -1.f;
	}
}

float UShooterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return BrakingDecelerationSliding;
	case CMOVE_WallRun:
		return 0.f;
	case CMOVE_Grappling:
	    return 0.f;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

bool UShooterMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() || IsWallRunning() || IsCrouching() || IsGrappling();
}

bool UShooterMovementComponent::DoJump(bool bReplayingMoves)
{
	//Need for pushing character when wall running and jump
	const bool bWasWallRunning = IsWallRunning(); //save this value because after super call player will be in falling
	const bool bWasGrappling = IsGrappling();
	
	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning)
		{
			FHitResult WallHit;
			IsWallOnSideTrace(WallHit,Safe_bWallRunIsRight);
			
			Velocity += WallHit.Normal * WallJumpOfForce;
		}
		if (bWasGrappling)
		{
			Velocity += (UpdatedComponent->GetForwardVector().GetSafeNormal() * GrapplingHorizontalJumpBoost) + (FVector::UpVector * GrapplingVerticalJumpBoost);
		}
		return true;
	}
	return false;
}

bool UShooterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UShooterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}
#pragma endregion

#pragma region Mantle
bool UShooterMovementComponent::TryMantle()
{
	FCollisionQueryParams CollisionQueryParams = ShooterCharacterOwner->GetIgnoreCharacterParams();
	const FVector BaseLoc = UpdatedComponent->GetComponentLocation() + FVector::DownVector * GetCapsuleHalfHeight(); 
	const FVector Fwd = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
	
	float CosMMAA = FMath::Cos(FMath::DegreesToRadians(MantleMaxAlignmentAngle));		 // max alignment angle cos
	float CosMMWSA = FMath::Cos(FMath::DegreesToRadians(MantleMinWallSteepnessAngle));  // min wall steepness cos
	float CosMMAXWSA = FMath::Cos(FMath::DegreesToRadians(MantleMaxWallSteepnessAngle));// max wall steepness cos
	float CosMMSA = FMath::Cos(FMath::DegreesToRadians(MantleMaxSurfaceAngle));	     // max top wall surface angle cos
	FName ProfileName("BlockAll");

	//Front trace & front wall angle checks
	FHitResult FrontHit;
	float CheckDistance = FMath::Clamp((Velocity | Fwd) * 0.8f, GetCapsuleRadius() + 30, MantleMaxDistance);
	FVector FrontStart = BaseLoc + FVector::UpVector * (MaxStepHeight -1);
	for (int i = 0; i < MantleRayCount; i++)
	{
		DEBUG_LINE(FrontStart,FrontStart + Fwd * CheckDistance,FColor::Red);
		if (GetWorld()->LineTraceSingleByProfile(FrontHit,FrontStart,FrontStart + Fwd*CheckDistance,ProfileName,CollisionQueryParams)) break;
		FrontStart += FVector::UpVector * (GetCapsuleHalfHeight() * 2.f - (MaxStepHeight - 1)) / MantleRayCount -1;
	}
	if (!FrontHit.IsValidBlockingHit()) return false;

	float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	if (CosWallSteepnessAngle > CosMMWSA || CosWallSteepnessAngle < CosMMAXWSA || (Fwd | -FrontHit.Normal) < CosMMAA) return false;
	DEBUG_POINT(FrontHit.Location, FColor::Green);

	//Top trace hit & top wall surface checks
	FVector WallUp = FVector::VectorPlaneProject(FVector::UpVector,FrontHit.Normal);
	const float WallSin = FMath::Sqrt(1 - CosWallSteepnessAngle * CosWallSteepnessAngle);

	FVector TopTraceStart = FrontHit.Location + Fwd + (WallUp * (MantleMaxWallHeight - (MaxStepHeight - 1)) * WallSin);
	DEBUG_LINE(TopTraceStart,FrontHit.Location + Fwd,FColor::Orange);

	TArray<FHitResult> TopWallHits;
	FHitResult TopWallSurfaceHit;
	GetWorld()->LineTraceMultiByProfile(TopWallHits,TopTraceStart,FrontHit.Location + Fwd,ProfileName,CollisionQueryParams);
	for (const FHitResult& TopHit : TopWallHits)
	{
		if (TopHit.IsValidBlockingHit())
		{
			TopWallSurfaceHit = TopHit;
			break;
		}
			
	}
	if (!TopWallSurfaceHit.IsValidBlockingHit() && (TopWallSurfaceHit.Normal | FVector::UpVector) < CosMMSA) return false;

	float WallHeight = (TopWallSurfaceHit.Location - BaseLoc) | FVector::UpVector;
	if (WallHeight > MantleMaxWallHeight) return false;

	//Capsule check
	float SurfaceCos = FVector::UpVector | TopWallSurfaceHit.Normal;
	float SurfaceSin = FMath::Sqrt(1 - SurfaceCos * SurfaceCos);
	
	FVector CapsuleLoc = TopWallSurfaceHit.Location + Fwd * GetCapsuleRadius() + FVector::UpVector * (GetCapsuleHalfHeight() + 1 + GetCapsuleRadius() * 2 * SurfaceSin);

	FCollisionShape CapShape = FCollisionShape::MakeCapsule(GetCapsuleRadius(),GetCapsuleHalfHeight());
	if (GetWorld()->OverlapAnyTestByProfile(CapsuleLoc,FQuat::Identity,ProfileName,CapShape,CollisionQueryParams))
	{
		DEBUG_CAPSULE(CapsuleLoc,FColor::Red);
		return false;
	}
	DEBUG_CAPSULE(CapsuleLoc,FColor::Green);

	//Mantle Selection
	EMantleType MantleType = EMantleType::Short;

	if (WallHeight > GetCapsuleHalfHeight() * 2)
	{
		if (IsMovementMode(MOVE_Walking))
		{
			MantleType = EMantleType::Tall;	
		}
		if (IsMovementMode(MOVE_Falling) && (Velocity | FVector::UpVector) < 0)
		{
			if (!GetWorld()->OverlapAnyTestByProfile(GetMantleStartLocation(FrontHit,TopWallSurfaceHit,EMantleType::Tall),FQuat::Identity,ProfileName,CapShape,CollisionQueryParams))
			{
				MantleType = EMantleType::Tall;
			}
		}
	}

	FVector TransitionTarget = GetMantleStartLocation(FrontHit,TopWallSurfaceHit,MantleType);

	DEBUG_CAPSULE(UpdatedComponent->GetComponentLocation(),FColor::White);
	DEBUG_CAPSULE(TransitionTarget,FColor::Blue);

	//Transition capsule to start location using root motion source
	const float UpSpeed = Velocity | FVector::UpVector;
	const float TransDistance = FVector::Dist(TransitionTarget,UpdatedComponent->GetComponentLocation());
	TransitionQueuedMontageSpeed = FMath::GetMappedRangeValueClamped(FVector2D(-500,750), FVector2D(.9f,1.2f),UpSpeed); 
	TransitionRMS.Reset();
	TransitionRMS = MakeShared<FRootMotionSource_MoveToForce>();
	TransitionRMS->AccumulateMode = ERootMotionAccumulateMode::Override;
	
	TransitionRMS->Duration = FMath::Clamp(TransDistance / 500.f,.1f,.25f);
	
	TransitionRMS->StartLocation = UpdatedComponent->GetComponentLocation();
	TransitionRMS->TargetLocation = TransitionTarget;

	//Apply transition root motion source
	Velocity = FVector::ZeroVector;
	SetMovementMode(MOVE_Flying);
	TransitionRMS_ID = ApplyRootMotionSource(TransitionRMS);
	TransitionName = ETransitionName::Mantle;
	
	//Animations
	UAnimMontage* TransitionMontage = nullptr;
	CurrentProxyMontage = nullptr;
	if (MantleType == EMantleType::Tall)
	{
		TransitionQueuedMontage = TallMantleAnimData.MantleMontage;
		TransitionMontage = TallMantleAnimData.TransitionMontage;
		CurrentProxyMontage = TallMantleAnimData.ProxyMontage;
	}
	else if (MantleType == EMantleType::Short)
	{
		TransitionQueuedMontage = ShortMantleAnimData.MantleMontage;
		TransitionMontage = ShortMantleAnimData.TransitionMontage;
		CurrentProxyMontage = ShortMantleAnimData.ProxyMontage;
	}

	if (TransitionMontage && CurrentProxyMontage)
	{
		ShooterCharacterOwner->PlayAnimMontage(TransitionMontage);
		if (ShooterCharacterOwner->HasAuthority())
		{
			Multicast_PlayMantleProxyAnim(CurrentProxyMontage);
		}
		CurrentProxyMontage = nullptr;
		CurrentProxyMontage = nullptr;
	}

	StartNewMovementAction(EMovementAction::Mantle);
	return true;
}

FVector UShooterMovementComponent::GetMantleStartLocation(const FHitResult& FrontHit, const FHitResult& SurfaceHit, EMantleType MantleType) const
{
	const float CosWallAngle = FrontHit.Normal | FVector::UpVector;
	float DownDistance = 0;
	
	if (MantleType == EMantleType::Short)
	{
		DownDistance = (MaxStepHeight - 1) - ShortMantleTransitionZOffset;
	}
	else if (MantleType == EMantleType::Tall)
	{
		DownDistance = (GetCapsuleHalfHeight() * 2.f) - TallMantleTransitionZOffset;
	}

	const FVector EdgeTangent = FVector::CrossProduct(SurfaceHit.Normal,FrontHit.Normal);

	FVector MantleStart = SurfaceHit.Location;
	MantleStart += FrontHit.Normal.GetSafeNormal2D() * (2.f + GetCapsuleRadius());
	MantleStart += UpdatedComponent->GetForwardVector().GetSafeNormal2D().ProjectOnTo(EdgeTangent) * GetCapsuleRadius() * .3f;
	MantleStart += FVector::UpVector * GetCapsuleHalfHeight();
	MantleStart += FVector::DownVector * DownDistance;
	MantleStart += FrontHit.Normal.GetSafeNormal2D() * CosWallAngle * DownDistance;
	
	return MantleStart;
}
#pragma endregion 


#pragma region Slide
bool UShooterMovementComponent::CanSlide() const
{
	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	
	const FName ProfileName = TEXT("BlockAll");
	
	const bool bValidSurface = GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, ShooterCharacterOwner->GetIgnoreCharacterParams());
	const bool bEnoughSpeed = Velocity.SizeSquared() > pow(MinSlideSpeed, 2);
	
	return bValidSurface && bEnoughSpeed /*IsFalling()*/;
}

void UShooterMovementComponent::EnterSlide()
{
	//CharacterOwner->bUseControllerRotationYaw = false;
	bWantsToCrouch = true;

	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);

}

void UShooterMovementComponent::ExitSlide()
{
	bWantsToCrouch = false;
}

void UShooterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	
	if (!CanSlide())
	{
		SetMovementMode(MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values
		UPrimitiveComponent * const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;

		FVector SlopeForce = CurrentFloor.HitResult.Normal;
		SlopeForce.Z = 0.f;
		Velocity += SlopeForce * SlideGravityForce * deltaTime;
		
		float MoveDirection = FVector::DotProduct(Velocity.GetSafeNormal(),GetCurrentAcceleration().GetSafeNormal());
		
		if (MoveDirection < -0.6f)
		{
			UnCrouch();
			SetMovementMode(MOVE_Walking);
			StartNewPhysics(deltaTime, Iterations);
			return;
		}

		Acceleration *= SlideAccelerationFactor;
		
		Velocity += Acceleration * deltaTime;
		ApplyVelocityBraking(deltaTime,GroundFriction * SlideFrictionFactor,GetMaxBrakingDeceleration());
		
		//DEBUG_LOG("Slide acceleration: %s | Move direction dot product: %f | Vel: %s" ,*Acceleration.ToString(),MoveDirection, *Velocity.ToString());
		//CalcVelocity(timeTick,GroundFriction * SlideFrictionFactor, false, GetMaxBrakingDeceleration());
		
		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity;
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;
		bool bFloorWalkable = CurrentFloor.IsWalkableFloor();

		if ( bZeroDelta )
		{
			remainingTime = 0.f;
		}
		else
		{
			// try to move forward
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if ( IsFalling() )
			{
				// pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if (DesiredDist > KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f,ActualDist/DesiredDist));
				}
				StartNewPhysics(remainingTime,Iterations);
				return;
			}
			else if ( IsSwimming() ) //just entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update floor.
		// StepUp might have already done it for us.
		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}


		// check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if ( bCheckLedges && !CurrentFloor.IsWalkableFloor() )
		{
			// calculate possible alternate movement
			const FVector GravDir = FVector(0.f,0.f,-1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
			if ( !NewDelta.IsZero() )
			{
				// first revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta / timeTick;
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// see if it is OK to jump
				// @todo collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ( (bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check
			if (CurrentFloor.IsWalkableFloor())
			{
				if (ShouldCatchAir(OldFloor, CurrentFloor))
				{
					HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);
					if (IsMovingOnGround())
					{
						// If still walking, then fall. If not, assume the user set a different mode they want to keep.
						StartFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
					}
					return;
				}

				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// check if just entered water
			if ( IsSwimming() )
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// See if we need to start falling.
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;
			}
		}
		
		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround() && bFloorWalkable)
		{
			// Make velocity reflect actual move
			if( !bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick;
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}

	FHitResult Hit;
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
}

bool UShooterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	
	return GetWorld()->LineTraceSingleByProfile(Hit,Start,End,FName("BlockAll"),ShooterCharacterOwner->GetIgnoreCharacterParams());
}

#pragma endregion

#pragma region Helpers
bool UShooterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

bool UShooterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

bool UShooterMovementComponent::IsWallOnSideTrace(FHitResult& WallHit,bool bWallRight) const
{
	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector RightTraceDistance = UpdatedComponent->GetRightVector() * GetCapsuleRadius() * 2;
	const FVector End = bWallRight ? Start + RightTraceDistance : Start - RightTraceDistance;
	
	return GetWorld()->LineTraceSingleByChannel(WallHit,Start,End,ECC_Visibility,ShooterCharacterOwner->GetIgnoreCharacterParams());
}

void UShooterMovementComponent::EnterMantle()
{
	CharacterOwner->bUseControllerRotationYaw = false;
}

void UShooterMovementComponent::ExitMantle()
{
	CharacterOwner->bUseControllerRotationYaw = true;
}

float UShooterMovementComponent::GetCapsuleRadius() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float UShooterMovementComponent::GetCapsuleHalfHeight() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

ACharacter* UShooterMovementComponent::GetDefaultCharacter() const
{
	return ShooterCharacterOwner ? ShooterCharacterOwner->GetClass()->GetDefaultObject<ACharacter>() : nullptr;
}
#pragma endregion

#pragma region WallRun

void UShooterMovementComponent::EnterWallRun()
{
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		FHitResult WallHit;
		Safe_bWallRunIsRight = IsWallOnSideTrace(WallHit,true);
	}
}

void UShooterMovementComponent::ExitWallRun()
{
}

bool UShooterMovementComponent::TryWallRun()
{
	/*If player is not falling or falling very quickly, he can not enter the wall run*/
	// TODO: put the conditions into separate variables?
	if (!IsFalling() || (Velocity.SizeSquared2D() < pow(MinWallRunSpeed,2)) || (Velocity.Z < -MaxVerticalWallRunSpeed) || (Velocity.Z < -MaxVerticalWallRunSpeed))
	{
		return false;
	}

	//Player height check
	FHitResult FloorHit;
	if (GetWorld()->LineTraceSingleByChannel(FloorHit,
		UpdatedComponent->GetComponentLocation(),
		UpdatedComponent->GetComponentLocation() + FVector::DownVector * (GetCapsuleHalfHeight() + MinWallRunHeight),
		ECC_Visibility,ShooterCharacterOwner->GetIgnoreCharacterParams()))
	{
		return false;
	}

	//Side casts . Velocity | WallHit.Normal must be less than 90 degrees to be able to enter wall run
	FHitResult WallHit;
	IsWallOnSideTrace(WallHit,false);
	if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0) 
	{
		Safe_bWallRunIsRight = false;
	}
	else
	{
		IsWallOnSideTrace(WallHit,true);
		if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
		{
			Safe_bWallRunIsRight = true;
		}
		else
		{
			return false;
		}
		
	}
	
	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity,WallHit.Normal);
	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z,0.f,MaxVerticalWallRunSpeed);
	
	SetMovementMode(MOVE_Custom,CMOVE_WallRun);
	
	return true;
}

void UShooterMovementComponent::PhysWallRun(float DeltaTime, int32 Iterations)
{
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}
	
	bJustTeleported = false;
	float remainingTime = DeltaTime;
	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		//
		FHitResult WallHit;
		IsWallOnSideTrace(WallHit,Safe_bWallRunIsRight);

		float SinPullAwayAngle = FMath::Sin(FMath::DegreesToRadians(WallRunPullAwayAngle));
		bool bWantsToPullAway = WallHit.IsValidBlockingHit() && !Acceleration.IsNearlyZero() && (Acceleration.GetSafeNormal() | WallHit.Normal) > SinPullAwayAngle;
		if (!WallHit.IsValidBlockingHit() || bWantsToPullAway)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime,Iterations);
			return;
		}
		
		Acceleration = FVector::VectorPlaneProject(Acceleration,WallHit.Normal);
		Acceleration.Z = 0;

		CalcVelocity(timeTick,0.f,false,GetMaxBrakingDeceleration());
		
		Velocity = FVector::VectorPlaneProject(Velocity,WallHit.Normal);

		float TangentAcceleration = Acceleration.GetSafeNormal() | Velocity.GetSafeNormal();
		bool bVelUp = Velocity.Z > 0.f;

		Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelUp ? 0.f : TangentAcceleration) * timeTick;
		
		if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed,2) || Velocity.Z < -MaxVerticalWallRunSpeed)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime,Iterations);
			return;
		}
		
		const FVector Delta = timeTick * Velocity;
		const bool bZeroDelta = Delta.IsNearlyZero();

		if (bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta,UpdatedComponent->GetComponentQuat(),true,Hit); //line that does all the movement. Keep same capsule rotation. sweep instead teleporting
			FVector	WallAttractionDelta = -WallHit.Normal * WallAttractionForce * timeTick;
			SafeMoveUpdatedComponent(WallAttractionDelta, UpdatedComponent->GetComponentQuat(), true, Hit);

		}

		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt . Need for losing velocity while player hits wall
	}

	FHitResult FloorHit, WallHit;
	IsWallOnSideTrace(WallHit, Safe_bWallRunIsRight);
	GetWorld()->LineTraceSingleByChannel(FloorHit,UpdatedComponent->GetComponentLocation(),
		UpdatedComponent->GetComponentLocation() + FVector::DownVector * (GetCapsuleHalfHeight() + MinWallRunHeight * .5f),
		ECC_Visibility,ShooterCharacterOwner->GetIgnoreCharacterParams());

	if (FloorHit.IsValidBlockingHit() || !WallHit.IsValidBlockingHit() || Velocity.SizeSquared2D() < pow(MinWallRunSpeed,2))
	{
		SetMovementMode(MOVE_Falling);
	}
}



#pragma endregion

#pragma region GrappingHook

void UShooterMovementComponent::EnterGrapple()
{
}

void UShooterMovementComponent::ExitGrapple()
{
	Safe_bWantsToGrapple = false;
	EnableGrapplingHookCableComponent(false);

	OnGrappleExit.Broadcast(ShooterCharacterOwner);
}

bool UShooterMovementComponent::TryGrapple()
{
	if (!ShooterCharacterOwner->IsLocallyControlled())
	{
		return false;
	}
	
	ShooterCharacterOwner->GetCableComponent()->SetVisibility(true);

	const FVector ProjectileDirection = ShooterCharacterOwner->GetGrapplingHookForwardVector();
	SpawnGrapplingHookProjectile(ShooterCharacterOwner->GetStartGrapplingHookLocation(),ShooterCharacterOwner->GetStartGrapplingHookLocation() + ProjectileDirection * GrapplingHookDistance);
	EnableGrapplingHookCableComponent(true);

	Server_TryGrapple(ProjectileDirection);
	
	return true;
}

void UShooterMovementComponent::StartGrappling(const FGrapplingHookAttachData& AttachData)
{
	GrapplingHookAttachData = AttachData;
	
	//Start grappling
	Server_SetGrapplingHookAttachPoint(AttachData);
	Safe_bWantsToGrapple = true;
}

AGrapplingHookProjectile* UShooterMovementComponent::SpawnGrapplingHookProjectile(const FVector& Location,
	const FVector& Direction)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = ShooterCharacterOwner;
	SpawnParams.Instigator = ShooterCharacterOwner;
	
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Location, Direction);
	
	GrapplingHookProjectile = GetWorld()->SpawnActor<AGrapplingHookProjectile>(ProjectileClass,Location,Rotation,SpawnParams);
	return GrapplingHookProjectile;
}

void UShooterMovementComponent::EnableGrapplingHookCableComponent(bool bEnabled)
{
	if (bEnabled)
	{
		ShooterCharacterOwner->GetCableComponent()->SetVisibility(true);
		ShooterCharacterOwner->GetCableComponent()->SetAttachEndTo(GrapplingHookProjectile,FName("None"));
		ShooterCharacterOwner->GetCableComponent()->EndLocation = FVector(0,0,0);
	}
	else
	{
		ShooterCharacterOwner->GetCableComponent()->SetVisibility(false);
	}

}


void UShooterMovementComponent::Server_SetGrapplingHookAttachPoint_Implementation(
	const FGrapplingHookAttachData& AttachData)
{
	GrapplingHookAttachData = AttachData;
}
void UShooterMovementComponent::Multicast_TryGrapple_Implementation(const FVector& ProjectileDirection,UCableComponent* CableComponent)
{
	if (ShooterCharacterOwner->IsLocallyControlled()) return;

	SpawnGrapplingHookProjectile(ShooterCharacterOwner->GetStartGrapplingHookLocation(),ShooterCharacterOwner->GetStartGrapplingHookLocation() + ProjectileDirection * GrapplingHookDistance);
	
	EnableGrapplingHookCableComponent(true);
}

void UShooterMovementComponent::Server_TryGrapple_Implementation(const FVector& ProjectileDirection)
{
	Multicast_TryGrapple(ProjectileDirection,ShooterCharacterOwner->GetCableComponent());
}

void UShooterMovementComponent::StopGrappling()
{
	if (ShooterCharacterOwner->IsLocallyControlled())
	{
		if (!IsGrappling())
		{
			ExitGrapple();
			Server_ExitGrapple();
		}
		Safe_bWantsToGrapple = false;
	}
}

void UShooterMovementComponent::OnRep_bInMantle(bool OldbInMantle)
{
	/*ShooterCharacterOwner->bUseControllerRotationYaw = OldbInMantle;
	if (ShooterCharacterOwner->bUseControllerRotationYaw)
	{
		DEBUG_SLOG("bUserControllerRotationYaw = true",FColor::Green);
	}
	else
	{
		DEBUG_SLOG("bUserControllerRotationYaw = false",FColor::Red);
	}*/
}

void UShooterMovementComponent::PhysGrappling(float DeltaTime, int32 Iterations)
{
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}
	
	bJustTeleported = false;
	float remainingTime = DeltaTime;
	
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		UE_LOG(LogTemp,Display,TEXT("GrapplingHookAttachData.AttachPoint: %s"),*GrapplingHookAttachData.AttachPoint.ToString());
		const FVector CharacterToAttachPointVec = GrapplingHookAttachData.AttachPoint - UpdatedComponent->GetComponentLocation();
		if ((CharacterToAttachPointVec.GetSafeNormal() | GrapplingHookAttachData.SurfaceNormal) > 0 || CharacterToAttachPointVec.Length() <= GrapplingReleasedDistance /*|| !Safe_bWantsToGrapple*/)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(DeltaTime, Iterations);
			//GEngine->AddOnScreenDebugMessage(-1,5,FColor::Blue,FString::Printf(TEXT("END GRAPPLING (CharacterToAttachPointVec.GetSafeNormal() | GrapplingHookAttachData.SurfaceNormal) : %f > 0 | CharacterToAttachPointVec.Length(): %f <= GrapplingReleasedDistance: %f"),(CharacterToAttachPointVec.GetSafeNormal() | GrapplingHookAttachData.SurfaceNormal), CharacterToAttachPointVec.Length(),GrapplingReleasedDistance));
			return;
		}

		Velocity = (CharacterToAttachPointVec.GetSafeNormal() * GetMaxSpeed()) * timeTick;

		Acceleration.Z = 0;
		
		Velocity += Acceleration * GrapplingSideAccelerationFactor * timeTick;
		Velocity.Z += GetGravityZ() * GrapplingGravityScale * DeltaTime;
		
		const FVector Delta = Velocity;
		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta,UpdatedComponent->GetComponentQuat(),true,Hit); //line that does all the movement. Keep same capsule rotation. sweep instead teleporting

		if (Hit.Time < 1.f)
		{
			HandleImpact(Hit,DeltaTime,Delta);
			SlideAlongSurface(Delta,1.f-Hit.Time,Hit.Normal,Hit,true);
		}

		
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt . Need for losing velocity while player hits wall
	}

	const FVector CharacterToAttachPointVec = GrapplingHookAttachData.AttachPoint - UpdatedComponent->GetComponentLocation();
	if ((CharacterToAttachPointVec.GetSafeNormal() | GrapplingHookAttachData.SurfaceNormal) > 0)
	{
		UE_LOG(LogTemp,Display,TEXT("Length: %f: "),CharacterToAttachPointVec.Length());
		SetMovementMode(MOVE_Falling);
		StartNewPhysics(DeltaTime, Iterations);
		return;
	}
	
}

void UShooterMovementComponent::SetDefaultRotationData()
{
	RotationTimeElapsed = 0.f;

	bCharacterRotatingLeft = false;
	bCharacterRotatingRight = false;
}

void UShooterMovementComponent::Server_ExitGrapple_Implementation()
{
	if (ShooterCharacterOwner->HasAuthority())
	{
		Multicast_ExitGrapple();
	}
}

void UShooterMovementComponent::Multicast_ExitGrapple_Implementation()
{
	if (ShooterCharacterOwner->IsLocallyControlled()) return;
	
	ExitGrapple();
}

void UShooterMovementComponent::Multicast_PlayMantleProxyAnim_Implementation(UAnimMontage* ProxyMontage)
{
	if (ShooterCharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		ShooterCharacterOwner->PlayAnimMontage(ProxyMontage);
	}
}

#pragma endregion
