// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS(Abstract)
class SIMPLESHOOTER_API UAttributeBase : public UAttributeSet
{
	GENERATED_BODY()
public:
	template <class T>
	T* GetAbilitySystemComponent() const;
};
template <class T>
T* UAttributeBase::GetAbilitySystemComponent() const
{
	return Cast<T>(GetOwningAbilitySystemComponent());
}