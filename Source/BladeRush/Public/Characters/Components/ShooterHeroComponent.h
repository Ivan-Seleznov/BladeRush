// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Components/PawnComponent.h"
#include "Input/ShooterInputConfig.h"
#include "ShooterHeroComponent.generated.h"

class UInputMappingContext;
class UInputComponent;
/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class BLADERUSH_API UShooterHeroComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	void InitializePlayerInput(UEnhancedInputComponent* PlayerInputComponent);

	FORCEINLINE UInputMappingContext* GetMappingContext() const {return  MappingContext;}

	void ChangeMappingContext(UInputMappingContext* NewMappingContext);
protected:
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* MappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UShooterInputConfig* InputConfig;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Input_QuickBarSlot1(const FInputActionValue& Value);
	void Input_QuickBarSlot2(const FInputActionValue& Value);
	void Input_QuickBarSlot3(const FInputActionValue& Value);

	void Input_QuickBarSlot0(const FInputActionValue& Value);

	void InventorySlotChanged(const float& SlotValue);
	//float CurrentSlotIndex = -1;
private:
	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(UEnhancedInputComponent* PlayerInputComponent,UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	template<class UserClass, typename FuncType>
	void BindNativeAction(UEnhancedInputComponent* PlayerInputComponent, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UShooterHeroComponent::BindAbilityActions(UEnhancedInputComponent* PlayerInputComponent, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	if (!PlayerInputComponent) return;
	
	for (const FShooterInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				PlayerInputComponent->BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				PlayerInputComponent->BindAction(Action.InputAction,ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}

template <class UserClass, typename FuncType>
void UShooterHeroComponent::BindNativeAction(UEnhancedInputComponent* PlayerInputComponent, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	if (!PlayerInputComponent) return;
	
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		UE_LOG(LogTemp,Display,TEXT("IA - %s | TagName %s"),*IA->GetName(), *InputTag.GetTagName().ToString())
		PlayerInputComponent->BindAction(IA, TriggerEvent, Object, Func);
	}
}	
