// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JA : ModuleRules
{
	public JA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
            "GameplayTags",
            "GameplayTasks",
            "AnimGraphRuntime",
            "MotionWarping",
            "Niagara",
            "NavigationSystem",
			"MoviePlayer",
			"UMG",
			"CommonInput",
			"PropertyPath",
			"PreLoadScreen"
        });

        if (Target.bBuildEditor)
        {
            // 에디터 툴 제작을 위한 전용 모듈
            PrivateDependencyModuleNames.AddRange(new string[] 
			{
				"UnrealEd",
				"AssetTools"
			});
        }

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
