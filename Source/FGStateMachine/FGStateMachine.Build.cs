using UnrealBuildTool;

public class FGStateMachine : ModuleRules
{
	public FGStateMachine(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BasicStateMachine" });
	}
}
