// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CURRENT_CLASS_FUNC (FString(__FUNCTION__))

#define CURRENT_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))))

#define CURRENT_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__)).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2))

#define CURRENT_LINE (FString::FromInt(__LINE__))

#define CURRENT_CLASS_LINE (CURRENT_CLASS + "(" + CURRENT_LINE + ")")

#define CURRENT_CLASS_FUNC_LINE (CURRENT_CLASS_FUNC + "(" + CURRENT_LINE + ")")

#define DEBUG_LOG(Format, ...)                                                                                           \
UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *CURRENT_CLASS_FUNC_LINE, *FString::Printf(TEXT(Format), ##__VA_ARGS__));

#define DEBUG_SLOG(x,c)		GEngine->AddOnScreenDebugMessage(-1,3.f,c,x);
#define DEBUG_POINT(x,c)	DrawDebugPoint(GetWorld(),x,10,c, false,3.f);
#define DEBUG_LINE(x1,x2,c) DrawDebugLine(GetWorld(),x1,x2,c,false,3.f);
#define DEBUG_CAPSULE(x,c)	DrawDebugCapsule(GetWorld(),x,GetCapsuleHalfHeight(),GetCapsuleRadius(),FQuat::Identity,c,false,3.f);

