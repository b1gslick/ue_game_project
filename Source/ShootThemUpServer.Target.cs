//This is My project
using UnrealBuildTool;
using System.Collections.Generic;
public class ShootThemUpServerTarget : TargetRules
{
public ShootThemUpServerTarget(TargetInfo Target) : base(Target)
{
Type = TargetType.Server;
ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
}
}
