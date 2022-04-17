// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WarriorsAdventure : ModuleRules
{
	public WarriorsAdventure(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "Niagara", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		PublicIncludePaths.AddRange(new string[] { "MinimapPlugin/Public", "MinimapPlugin/Private" });

		PublicDependencyModuleNames.AddRange(new string[] { "MinimapPlugin" });

		PrivateIncludePaths.Add(ModuleDirectory);
		PublicIncludePaths.Add(ModuleDirectory);
	}
}
