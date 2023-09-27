
#include "Data/GameTags.h"
#include "GameplayTagsManager.h"

FGameTags FGameTags::GameTags;

FGameplayTag FGameTags::FindTagByString(FString TagString, bool bMatchPartialString /*= false*/)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}

void FGameTags::AddTags()
{
	/*InputTags*/
	/*NativeInputTags*/
	AddTag(InputTag_Move, "InputTag.Move", "On activate Move");
	AddTag(InputTag_Look, "InputTag.Look", "On activat Look");

	
	/*AbilityInputTags*/
	AddTag(InputTag_Abilities_Sprint, "InputTag.Abilities.Sprint", "On activation ability sprint.");
	AddTag(InputTag_Abilities_Crouch, "InputTag.Abilities.Crouch", "On activation ability crouch.");
	AddTag(InputTag_Abilities_Jump, "InputTag.Abilities.Jump", "On activation ability jump.");
	AddTag(InputTag_Abilities_TryGrappleHook,"InputTag.Abilities.TryGrappleHook","On activation ability TryGrappleHook.");

	AddTag(Disable_Ability_Sprint,"Disable.Ability.Sprint","Disabled sprint ability");
}

void FGameTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FGameTags::AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode)
{
	AddTag(OutTag, TagName, "Character movement mode tag.");
	GameTags.MovementModeTagMap.Add(MovementMode, OutTag);
}

void FGameTags::AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode)
{
	AddTag(OutTag, TagName, "Character custom movement mode tag.");
	GameTags.CustomMovementModeTagMap.Add(CustomMovementMode, OutTag);
}
