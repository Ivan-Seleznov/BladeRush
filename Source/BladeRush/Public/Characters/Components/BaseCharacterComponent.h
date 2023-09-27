// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Components/ActorComponent.h"
#include "BaseCharacterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADERUSH_API UBaseCharacterComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UBaseCharacterComponent();

	/** Gets the character that owns the component, this will always be valid during gameplay but can return null in the editor */
	template <class T>
	T* GetCharacter() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, ABaseCharacter>::Value, "'T' template parameter to GetCharacter must be derived from ABaseCharacter");
		return Cast<T>(GetOwner());
	}

	template <class T>
	T* GetCharacterChecked() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, ABaseCharacter>::Value, "'T' template parameter to GetCharacterChecked must be derived from ABaseCharacter");
		return CastChecked<T>(GetOwner());
	}

	/** Gets the player state that owns the component, this can return null on clients for player characters that are still being replicated */
	template <class T>
	T* GetPlayerState() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APlayerState>::Value, "'T' template parameter to GetPlayerState must be derived from APlayerState");
		return GetCharacterChecked<ABaseCharacter>()->GetPlayerState<T>();
	}

	/** Gets the controller that owns the component, this will usually be null on clients */
	template <class T>
	T* GetController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' template parameter to GetController must be derived from AController");
		return GetCharacterChecked<ABaseCharacter>()->GetController<T>();
	}
};
