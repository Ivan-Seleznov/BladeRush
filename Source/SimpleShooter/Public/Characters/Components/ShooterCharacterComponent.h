// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Characters/Components/BaseCharacterComponent.h"
#include "InputActionValue.h"
#include "Input/ShooterInputConfig.h"
#include "ShooterCharacterComponent.generated.h"

class UInputMappingContext;
class UInputComponent;
/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class SIMPLESHOOTER_API UShooterCharacterComponent : public UBaseCharacterComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void InitCharacter();

protected:
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* MappingContext;
	
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	//@TODO: Must be in custom input component

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UShooterInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UShooterInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UShooterCharacterComponent::BindAbilityActions(const UShooterInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character)
	{
		return;
	}
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Character->InputComponent);
	if (!Input)
	{
		return;
	}
	
	for (const FShooterInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				Input->BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				Input->BindAction(Action.InputAction,ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}

template <class UserClass, typename FuncType>
void UShooterCharacterComponent::BindNativeAction(const UShooterInputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	ABaseCharacter* Character = GetCharacter<ABaseCharacter>();
	if (!Character)
	{
		return;
	}
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Character->InputComponent);
	if (!Input)
	{
		return;
	}
	
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		UE_LOG(LogTemp,Display,TEXT("IA - %s | TagName %s"),*IA->GetName(), *InputTag.GetTagName().ToString())
		Input->BindAction(IA, TriggerEvent, Object, Func);
	}
}	
