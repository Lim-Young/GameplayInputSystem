using UnrealBuildTool;

public class GameplayInputForwardingSystem : ModuleRules
{
    public GameplayInputForwardingSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "GameplayInputSystem", 
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayAbilities",
                "GameplayTags",
                "DeveloperSettings"
            }
        );
    }
}