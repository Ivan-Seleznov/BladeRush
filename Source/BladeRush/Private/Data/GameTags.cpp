
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
	AddTag(InputTag_Look, "InputTag.Look", "On activate Look");
	
	AddTag(InputTag_QuickBar_Slot1, "InputTag.QuickBar.Slot1", "QuickSlot 1");
	AddTag(InputTag_QuickBar_Slot2, "InputTag.QuickBar.Slot2", "QuickSlot 2");
	AddTag(InputTag_QuickBar_Slot3, "InputTag.QuickBar.Slot3", "QuickSlot 3");
	AddTag(InputTag_QuickBar_Slot0, "InputTag.QuickBar.Slot0", "QuickSlot 0");
	
	/*AbilityInputTags*/
	AddTag(InputTag_Abilities_Sprint, "InputTag.Abilities.Sprint", "On activation ability sprint.");
	AddTag(InputTag_Abilities_Crouch, "InputTag.Abilities.Crouch", "On activation ability crouch.");
	AddTag(InputTag_Abilities_Jump, "InputTag.Abilities.Jump", "On activation ability jump.");
	AddTag(InputTag_Abilities_TryGrappleHook,"InputTag.Abilities.TryGrappleHook","On activation ability TryGrappleHook.");
	AddTag(InputTag_Abilities_Weapon_Fire,"InputTag.Abilities.Weapon.Fire","On activation weapon fire ability");
	AddTag(InputTag_Abilities_ADS,"InputTag.Abilities.ADS","On activation ads ability");
	AddTag(InputTag_Abilities_Weapon_Reload,"InputTag.Abilities.Weapon.Reload","On activation weapon reload ability");

	AddTag(Disable_Ability_Sprint,"Disable.Ability.Sprint","Disabled sprint ability");


	/*cost gameplay tags*/
	AddTag(Cost_Ability_TryGrapple,"Cost.Ability.TryGrapple","TryGrapple ability cost");

	AddTag(Data_Cooldown,"Data.Cooldown","Ability cooldown");
	
	AddTag(Data_Cooldown_TryGrapple,"Data.Cooldown.TryGrapple","Data Cooldown TryGrapple");

	AddTag(Ability_Sprint,"Ability.Sprint","Sprint ability");
	AddTag(Ability_Weapon_ADS,"Ability.Weapon.ADS","ADS weapon ability");
	AddTag(Ability_Weapon_Fire,"Ability.Weapon.Fire","Weapon fire ability");

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
