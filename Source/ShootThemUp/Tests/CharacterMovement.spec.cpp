// This is My project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ShootThemUp/Tests/TestUtils.h"
#include "ShootThemUp/Public/Player/STUBaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"

// DEFINE_SPEC(FTestCharacterFastRunning, "ShootThemUp.Character.Movement.TestCharacterFastRunning",
//    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

// DEFINE_SPEC(FTestClassName, "Category.TestName",
//    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

BEGIN_DEFINE_SPEC(FTestCharacterMovement, "ShootThemUp.Character.Movement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

END_DEFINE_SPEC(FTestCharacterMovement)

using namespace ShootThemUp::Test;

namespace
{
constexpr char* MapName = "/Game/Levels/TestLevel";
constexpr char* Character_bp = "/Script/Engine.Blueprint'/Game/Player/BP_STUBaseCharacter.BP_STUBaseCharacter'";
const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
}  // namespace

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FFastRunLatentCommand, UInputComponent*, InputComponent);
bool FFastRunLatentCommand::Update()
{
    FastRunPressed(InputComponent);
    return true;
}

void FTestCharacterMovement::Define()
{
    Describe("Test",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("Map not exists", World);
                });
            It("Test",
                [this]()
                {
                    ASTUBaseCharacter* SUT_Character = CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;

                    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
                    TestTrueExpr(PC != nullptr);

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFastRunLatentCommand(SUT_Character->InputComponent));

                    return true;
                });
        });
}

// /Script/Engine.World'/Game/Levels/TestLevel.TestLevel'
// /Script/Engine.Blueprint'/Game/Player/BP_STUBaseCharacter.BP_STUBaseCharacter'

#endif