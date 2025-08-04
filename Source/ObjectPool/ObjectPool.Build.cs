// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ObjectPool : ModuleRules
{
	public ObjectPool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayTags"
		});
	}
}
