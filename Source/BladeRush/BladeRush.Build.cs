// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BladeRush : ModuleRules
{
	public BladeRush(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","EnhancedInput"});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
		"GameplayAbilities",
		 "GameplayTags",	
		 "GameplayTasks",
		 "Slate", "SlateCore",
		 /*"OnlineSubsystem",*/
		 "ModularGameplay"
		});
	}
}
