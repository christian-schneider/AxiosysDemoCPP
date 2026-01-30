// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AxiosysDemo : ModuleRules
{
	public AxiosysDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AxiosysDemo",
			"AxiosysDemo/Variant_Horror",
			"AxiosysDemo/Variant_Horror/UI",
			"AxiosysDemo/Variant_Shooter",
			"AxiosysDemo/Variant_Shooter/AI",
			"AxiosysDemo/Variant_Shooter/UI",
			"AxiosysDemo/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
