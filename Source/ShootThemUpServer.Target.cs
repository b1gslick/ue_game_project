//This is My project
using UnrealBuildTool;
using System.Collections.Generic;
public class ShootThemUpTarget : TargetRules
{
public ShootThemUpTarget(TargetInfo Target) : base(Target)
{
Type = TargetType.Server;
ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
}
}
