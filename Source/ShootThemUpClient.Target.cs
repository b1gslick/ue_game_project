//This is My project
using UnrealBuildTool;
using System.Collections.Generic;
public class ShootThemUpClientTarget : TargetRules
{
public ShootThemUpClientTarget(TargetInfo Target) : base(Target)
{
Type = TargetType.Client;
ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
}
}
