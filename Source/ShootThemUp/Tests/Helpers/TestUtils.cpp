// Shoot Them Up game, All right resievied

#include "Helpers/TestUtils.h"
#include "BufferVisualizationData.h"
#include "AutomationBlueprintFunctionLibrary.h"  // add FunctionalTesting to PublicDependencyModuleNames in your .Build.cs file
#include "Kismet/GameplayStatics.h"


namespace ShootThemUp
{
namespace Test
{
UWorld* GetTestGameWorld()
{

    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if (((Context.WorldType == EWorldType::PIE) || Context.WorldType == EWorldType::Game) && Context.World())
        {
            return Context.World();
        }
    }

    return nullptr;
};

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (!Object) return;

    FString Command = FString::Printf(TEXT("%s"), *FuncName);
    for (const auto Param : Params)
    {
        Command.Append(" ").Append(Param);
    }

    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);

    // FString::Printf(TEXT("X=%3.3f Y=%3.3f Z=%3.3f"), X, Y, Z); toString
    //"SetTestData 123 345.34 \"My cool string\" Yes ";
    // first args name function than variables separator space
}

int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent)
{
    if (!InputComp) return INDEX_NONE;

    for (int32 i = 0; i < InputComp->GetNumActionBindings(); ++i)
    {
        const FInputActionBinding Action = InputComp->GetActionBinding(i);
        if (Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
        {
            return i;
        }
    }

    return INDEX_NONE;
}

int32 GetAxisBindingIndexByName(UInputComponent* InputComp, const FString& AxisName)
{
    if (!InputComp) return INDEX_NONE;

    const int32 AxisIndex = InputComp->AxisBindings.IndexOfByPredicate(
        [=](const FInputAxisBinding& AxisBind) { return AxisBind.AxisName.ToString().Equals(AxisName); });

    return AxisIndex;
}

void DoInputAction(UInputComponent* InputComponent, const FString& ActionName, const FKey& Key)
{
    if (!InputComponent) return;
    const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, ActionName, EInputEvent::IE_Pressed);
    if (ActionIndex != -1)
    {
        const auto ActionBind = InputComponent->GetActionBinding(ActionIndex);
        ActionBind.ActionDelegate.Execute(Key);
    }
}


void JumpPressed(UInputComponent* InputComponent)
{
    DoInputAction(InputComponent, "Jump", EKeys::SpaceBar);
}

void SpecCloseLevel()
{

    if (UWorld* World = GetTestGameWorld())
    {
        if (APlayerController* TargetPC = UGameplayStatics::GetPlayerController(World, 0))
        {
            TargetPC->ConsoleCommand(TEXT("Exit"), true);
        }
    }
}
}  // namespace Test
}  // namespace ShootThemUp