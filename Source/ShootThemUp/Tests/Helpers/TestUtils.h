// Shoot Them Up game, All right resievied

#pragma once

#if WITH_AUTOMATION_TESTS
//#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "Tests/AutomationCommon.h"
#include "Misc/OutputDeviceNull.h"

namespace ShootThemUp
{
namespace Test
{
template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    // load blueprint
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);

    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform)
                                : nullptr;  // Spawn blueprint if pointer is not null, else return null
}

template <typename T>
T* CreateBlueprintDeferred(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    // load blueprint
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);

    return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform)
                                : nullptr;  // Spawn blueprint if pointer is not null, else return null
}
void JumpPressed(UInputComponent* InputComponent);

class LevelScope
{
public:
    LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

constexpr char* MapName = "/Game/Tests/MAP_FeatureTest";
constexpr char* Character_bp = "/Script/Engine.Blueprint'/Game/Tests/BP_STUTestPlayerCharacter.BP_STUTestPlayerCharacter'";
const FTransform InitialTransform{FVector{1280.0f, -1486.0f, 230.0f}};


UWorld* GetTestGameWorld();

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent);
int32 GetAxisBindingIndexByName(UInputComponent* InputComp, const FString& AxisName);
void DoInputAction(UInputComponent* InputComponent, const FString& ActionName, const FKey& Key);

void FastRunPressed(UInputComponent* InputComponent);

void SpecCloseLevel();

}  // namespace Test
}  // namespace ShootThemUp

#endif