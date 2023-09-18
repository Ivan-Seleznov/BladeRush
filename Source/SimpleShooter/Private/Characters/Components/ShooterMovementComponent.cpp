// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ShooterMovementComponent.h"

#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

bool UShooterMovementComponent::FSavedMove_Shooter::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                   ACharacter* InCharacter, float MaxDelta) const
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

	//Saved_bHadAnimRootMotion = ShooterMovementComponent->Safe_bHadAnimRootMotion;
	//Saved_bTransitionFinished = ShooterMovementComponent->Safe_bTransitionFinished;
}

void UShooterMovementComponent::FSavedMove_Shooter::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UShooterMovementComponent* ShooterMovementComponent = Cast<UShooterMovementComponent>(C->GetCharacterMovement());
	
	ShooterMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
	ShooterMovementComponent->Safe_bWantsToSlide = Saved_bWantsToSlide;
	
	ShooterMovementComponent->ShooterCharacterOwner->bPlayerPressedJump = Saved_bPressedPlayerJump;
	
	//ShooterMovementComponent->Safe_bHadAnimRootMotion = Saved_bHadAnimRootMotion;
	//ShooterMovementComponent->Safe_bTransitionFinished = Saved_bTransitionFinished;
}

void UShooterMovementComponent::FSavedMove_Shooter::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
	Saved_bPressedPlayerJump = 0;

	//Saved_bHadAnimRootMotion = 0;
	//Saved_bTransitionFinished = 0;
}

UShooterMovementComponent::FNetworkPredictionDataClient_Shooter::FNetworkPredictionDataClient_Shooter(const UCharacterMovementComponent& ClientMovement)
: FNetworkPredictionData_Client_Character(ClientMovement)
{
}

FSavedMovePtr UShooterMovementComponent::FNetworkPredictionDataClient_Shooter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Shooter());
}

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

bool UShooterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

bool UShooterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
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

bool UShooterMovementComponent::CanSprint() const
{
	return IsMovementMode(MOVE_Walking) && MoveVector.Y > 0.f;
}

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

bool UShooterMovementComponent::CanSlide() const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	bool bValidSurface = GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, ShooterCharacterOwner->GetIgnoreCharacterParams());
	bool bEnoughSpeed = Velocity.SizeSquared() > pow(MinSlideSpeed, 2);
	
	return bValidSurface && bEnoughSpeed && !IsMovementMode(MOVE_Falling);
}

void UShooterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, MoveVector);
}

void UShooterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Shooter::FLAG_Custom_0) != 0;
}

ACharacter* UShooterMovementComponent::GetDefaultCharacter() const
{
	return ShooterCharacterOwner ? ShooterCharacterOwner->GetClass()->GetDefaultObject<ACharacter>() : nullptr;
}

bool UShooterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
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

bool UShooterMovementComponent::TryMantle() const
{
	return false;
}

bool UShooterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
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
