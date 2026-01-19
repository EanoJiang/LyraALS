using UnrealBuildTool;

public class AnimNotifyBatchTool : ModuleRules
{
    public AnimNotifyBatchTool(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "PropertyEditor",
                "LevelEditor"
            }
        );
    }
}
