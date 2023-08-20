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

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FFastRunLatentCommand, UInputComponent*, InputComponent);

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FNewJumpLatentCommand, UInputComponent*, InputComponent);
bool FNewJumpLatentCommand::Update()
{
    JumpPressed(InputComponent);
    return true;
}

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
            /*          It("Test",
                          [this]()
                          {
                              ASTUBaseCharacter* SUT_Character = CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp,
               InitialTransform); if (!TestNotNull("Character exist", SUT_Character)) return false;

                              APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
                              TestTrueExpr(PC != nullptr);

                              ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));
                              ADD_LATENT_AUTOMATION_COMMAND(FFastRunLatentCommand(SUT_Character->InputComponent));

                              return true;
                          });*/

            It("Jump",
                [this]()
                {
                    const FTransform LandedInitialTransform{FVector{1200.0f, -600.0f, 100.0f}};
                    ASTUBaseCharacter* SUT_Character =
                        CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp, LandedInitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    SUT_Character->FinishSpawning(LandedInitialTransform);
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f))
                    ADD_LATENT_AUTOMATION_COMMAND(FNewJumpLatentCommand(SUT_Character->InputComponent));

                    return true;
                });

            AfterEach([this]() { SpecCloseLevel(World); });
        });
}

// /Script/Engine.World'/Game/Levels/TestLevel.TestLevel'
// /Script/Engine.Blueprint'/Game/Player/BP_STUBaseCharacter.BP_STUBaseCharacter'

#endif