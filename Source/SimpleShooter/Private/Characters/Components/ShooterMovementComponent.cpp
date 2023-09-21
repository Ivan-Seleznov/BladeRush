// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ShooterMovementComponent.h"

#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#if	1
float MacroDuration = 3.f;
#define SLOG(x,c)			GEngine->AddOnScreenDebugMessage(-1,MacroDuration,c,x);
#define DEBUG_POINT(x,c)	DrawDebugPoint(GetWorld(),x,10,c, false,MacroDuration);
#define DEBUG_LINE(x1,x2,c) DrawDebugLine(GetWorld(),x1,x2,c,false,MacroDuration);
#define DEBUG_CAPSULE(x,c)	DrawDebugCapsule(GetWorld(),x,GetCapsuleHalfHeight(),GetCapsuleRadius(),FQuat::Identity,c,false,MacroDuration);
#else
#define SLOG(x,c)
#define DEBUG_POINT(x,c)
#define DEBUG_LINE(x1,x2,c)
#define DEBUG_CAPSULE(x,c)
#endif

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
}
void UShooterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, MoveVector);
}


bool UShooterMovementComponent::CanSprint() const
{
	return IsMovementMode(MOVE_Walking) && MoveVector.Y > 0.f;
}

#pragma region MoveVector
void UShooterMovementComponent::Client_SetMoveVector_Implementation(const FVector2D& NewValue)
{
	if (ShooterCharacterOwner->HasAuthority())
	{
		// This function should only be called by clients
		return;
	}

	// Apply the new move vector received from the client
	MoveVector.X = NewValue.X;
	MoveVector.Y = NewValue.Y;

	// Call the server function to update the variable on the server
	Server_SetMoveVector(NewValue);
}

void UShooterMovementComponent::Server_SetMoveVector_Implementation(const FVector2D& NewValue)
{
	MoveVector.X = NewValue.X;
	MoveVector.Y = NewValue.Y;
}
#pragma endregion

#pragma region CMC
bool UShooterMovementComponent::FSavedMove_Shooter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Shooter* NewShooterMove = static_cast<FSavedMove_Shooter*>(NewMove.Get());
	if (Saved_bWantsToSprint != NewShooterMove->Saved_bWantsToSprint)
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
}

void UShooterMovementComponent::FSavedMove_Shooter::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
	Saved_bPressedPlayerJump = 0;

	Saved_bPressedPlayerJump = 0;
	Saved_bHadAnimRootMotion = 0;
	Saved_bTransitionFinished = 0;
}

void UShooterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Shooter::FLAG_Custom_0) != 0;
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

	
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide)
	{
		ExitSlide();
		if (IsMovementMode(MOVE_Walking))
		{
			UE_LOG(LogTemp,Display,TEXT("Current movement mode: WALKING"))
		}
	}
	
	if (IsCustomMovementMode(CMOVE_Slide))
	{
		EnterSlide();
	}
}

void UShooterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	if (!HasAnimRootMotion() && Safe_bHadAnimRootMotion && IsMovementMode(MOVE_Flying))
	{
		SetMovementMode(MOVE_Walking);
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
	
	if (ShooterCharacterOwner->bPlayerPressedJump)
	{
		if (TryMantle())
		{
			ShooterCharacterOwner->StopJumping();
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

	if (Safe_bTransitionFinished)
	{
		SLOG(TEXT("Transition Finished"),FColor::Blue);
		if (TransitionName == ETransitionName::TNAME_Mantle)
		{
			if (IsValid(TransitionQueuedMontage) && TransitionQueuedMontage->HasRootMotion())
			{
				SetMovementMode(MOVE_Flying);
				CharacterOwner->PlayAnimMontage(TransitionQueuedMontage,TransitionQueuedMontageSpeed);
				TransitionQueuedMontageSpeed = 0.f;
				TransitionQueuedMontage = nullptr;
			}
			else
			{
				SetMovementMode(MOVE_Walking);
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
	default:
		UE_LOG(LogTemp, Fatal,TEXT("Invalid Movement Mode"));
		break;
	}
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
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
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
		if (TopHit.IsValidBlockingHit()) TopWallSurfaceHit = TopHit;
	}
	if (!TopWallSurfaceHit.IsValidBlockingHit() && (TopWallSurfaceHit.Normal | FVector::UpVector) < CosMMSA) return false;

	float WallHeight = (TopWallSurfaceHit.Location - BaseLoc) | FVector::UpVector;
	if (WallHeight > MantleMaxWallHeight) return false;

	//Capsule check
	float SurfaceCos = FVector::UpVector | TopWallSurfaceHit.Normal;
	float SurfaceSin = FMath::Sqrt(1- SurfaceCos * SurfaceCos);
	
	FVector CapsuleLoc = TopWallSurfaceHit.Location + Fwd * GetCapsuleRadius() + FVector::UpVector * (GetCapsuleHalfHeight() + 1 + GetCapsuleRadius() * 2 * SurfaceSin);

	FCollisionShape CapShape = FCollisionShape::MakeCapsule(GetCapsuleRadius(),GetCapsuleHalfHeight());
	if (GetWorld()->OverlapAnyTestByProfile(CapsuleLoc,FQuat::Identity,ProfileName,CapShape,CollisionQueryParams))
	{
		DEBUG_CAPSULE(CapsuleLoc,FColor::Red);
	}
	else DEBUG_CAPSULE(CapsuleLoc,FColor::Green);

	//Mantle Selection
	EMantleType MantleType = EMantleType::TMANTLE_Short;

	if (WallHeight > GetCapsuleHalfHeight() * 2)
	{
		if (IsMovementMode(MOVE_Walking))
		{
			MantleType = EMantleType::TMANTLE_Tall;	
		}
		if (IsMovementMode(MOVE_Falling) && (Velocity | FVector::UpVector) < 0)
		{
			if (!GetWorld()->OverlapAnyTestByProfile(GetMantleStartLocation(FrontHit,TopWallSurfaceHit,EMantleType::TMANTLE_Tall),FQuat::Identity,ProfileName,CapShape,CollisionQueryParams))
			{
				MantleType = EMantleType::TMANTLE_Tall;
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
	TransitionName = ETransitionName::TNAME_Mantle;
	
	//Animations
	
	
	return true;
}

FVector UShooterMovementComponent::GetMantleStartLocation(const FHitResult& FrontHit, const FHitResult& SurfaceHit, EMantleType MantleType) const
{
	float CosWallAngle = FrontHit.Normal | FVector::UpVector;
	float DownDistance = 0;
	if (MantleType == EMantleType::TMANTLE_Short) DownDistance = MaxStepHeight - 1;
	else if (MantleType == EMantleType::TMANTLE_Tall) DownDistance = GetCapsuleHalfHeight() * 2.f;

	FVector EdgeTangent = FVector::CrossProduct(SurfaceHit.Normal,FrontHit.Normal);
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Green,FString::Printf(TEXT("Edge Tangent: %s"),*EdgeTangent.ToString()));

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
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	bool bValidSurface = GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, ShooterCharacterOwner->GetIgnoreCharacterParams());
	bool bEnoughSpeed = Velocity.SizeSquared() > pow(MinSlideSpeed, 2);
	
	return bValidSurface && bEnoughSpeed && !IsMovementMode(MOVE_Falling);
}

void UShooterMovementComponent::EnterSlide()
{
	CharacterOwner->bUseControllerRotationYaw = false;
	bWantsToCrouch = true;

	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);

	UE_LOG(LogTemp,Warning,TEXT("EnterSlide"));
}

void UShooterMovementComponent::ExitSlide()
{
	CharacterOwner->bUseControllerRotationYaw = true;
	bWantsToCrouch = false;
	
	UE_LOG(LogTemp,Display,TEXT("ExitSlide"));
}

void UShooterMovementComponent:: PhysSlide(float deltaTime, int32 Iterations)
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
		
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector().GetSafeNormal2D());

		// Apply acceleration
		CalcVelocity(timeTick, GroundFriction * SlideFrictionFactor, false, GetMaxBrakingDeceleration());
		
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