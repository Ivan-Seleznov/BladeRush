#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FGameTags : public FGameplayTagNativeAdder
{
	virtual ~FGameTags(){};
	
public:
	static const FGameTags& Get() { return GameTags; }

	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);

protected:
	virtual void AddTags() override;
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode);
	void AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode);

public:

	/*Input tags*/
	FGameplayTag InputTag_Abilities_Sprint;
	FGameplayTag InputTag_Abilities_Crouch;
	FGameplayTag InputTag_Abilities_Jump;
	FGameplayTag InputTag_Abilities_TryGrappleHook;

	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look;

	FGameplayTag Cost_Ability_TryGrapple;
	
	FGameplayTag Data_Cooldown;
	
	FGameplayTag Data_Cooldown_TryGrapple;
	
	TMap<uint8, FGameplayTag> MovementModeTagMap;
	TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	FGameplayTag Disable_Ability_Sprint;

	FGameplayTag Ability_Sprint;
	
private:
	static FGameTags GameTags;
};
