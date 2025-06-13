using UnrealBuildTool;

public class GameplayInteractionSystem : ModuleRules
{
    public GameplayInteractionSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayInputSystem",
                "GameplayTags",
                "UMG"
            }
        );
    }
}