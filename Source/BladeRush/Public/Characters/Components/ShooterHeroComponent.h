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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bAllowSwitchSameSlot = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bUseSwitchSlotTimer = true;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(EditCondition="bUseSwitchSlotTimer"))
	float SwitchSlotTimerTime = 0.2f;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Input_QuickBarSlot1(const FInputActionValue& Value);
	void Input_QuickBarSlot2(const FInputActionValue& Value);
	void Input_QuickBarSlot3(const FInputActionValue& Value);

	void Input_QuickBarSlot0(const FInputActionValue& Value);

	bool TryChangeInventorySlot(int32 SlotIndex);
	void InventorySlotChanged(int32 SlotIndex);
private:
	UPROPERTY()
	FTimerHandle SlotChangedClientTimerHandle;
	UPROPERTY()
	int32 CurrentSlotIndex = -2;

	int32 PrevSlotIndexToChange = 0;
	
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
				ETriggerEvent PressedTriggerEvent = ETriggerEvent::Triggered;
				if (Action.bAdvancedBindAction)
				{
					PressedTriggerEvent = ETriggerEvent::Started;
				}

				PlayerInputComponent->BindAction(Action.InputAction, PressedTriggerEvent, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				ETriggerEvent ReleaseTriggerEvent = ETriggerEvent::Completed;
				if (Action.bAdvancedBindAction)
				{
					ReleaseTriggerEvent = ETriggerEvent::Triggered;
				}
				
				PlayerInputComponent->BindAction(Action.InputAction,ReleaseTriggerEvent, Object, ReleasedFunc, Action.InputTag);
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
