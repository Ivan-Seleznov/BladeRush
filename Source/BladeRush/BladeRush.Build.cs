// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BladeRush : ModuleRules
{
	public BladeRush(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","EnhancedInput","DataRegistry","ModularGameplay", "GameplayAbilities"});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
		 "GameplayTags",	
		 "GameplayTasks",
		 "Slate", "SlateCore",
		 "CableComponent",
		 "NetCore",
		 "PhysicsCore", 
		 "Niagara"
		});
	}
}
