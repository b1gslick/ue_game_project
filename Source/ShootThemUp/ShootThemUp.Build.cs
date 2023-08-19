// Shoot Them Up game, All right resievied

using UnrealBuildTool;

public class ShootThemUp : ModuleRules
{
    public ShootThemUp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "FunctionalTesting",
            "Niagara",
            "PhysicsCore",
            "GameplayTasks",
            "NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
        {
            "ShootThemUp/Public/Player",
            "ShootThemUp/Public/Components",
            "ShootThemUp/Tests",
            "ShootThemUp/Public/Dev",
            "ShootThemUp/Public/Weapon",
            "ShootThemUp/Public/Weapon/Components",
            "ShootThemUp/Public/UI",
            "ShootThemUp/Public/Animations",
            "ShootThemUp/Public/Pickpups",
            "ShootThemUp/AI",
            "ShootThemUp/AI/Tasks"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
