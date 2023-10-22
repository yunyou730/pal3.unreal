// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Test1 : ModuleRules
{
	public Test1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", "CoreUObject", "Engine", "InputCore" ,
				"CustomMeshComponent",
				"ProceduralMeshComponent",
				"ImageWrapper"
			}
		);

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		bEnableUndefinedIdentifierWarnings = false;

		PublicIncludePaths.AddRange(
			new string[] {
				"Test1",
				"Test1/Pal3Core/headers/lzo"
			}
		);
	}
}
