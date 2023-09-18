// This is My project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Helpers/TestUtils.h"
#include "ShootThemUp/Public/Player/STUBaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/STUTestItem.h"
#include "STUUtils.h"
#include "Components/SphereComponent.h"


BEGIN_DEFINE_SPEC(FTestCharacterMovement, "ShootThemUp.Character.Movement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
static constexpr char* TestItemName = "/Script/Engine.Blueprint'/Game/Tests/BP_STUTestItem.BP_STUTestItem'";

END_DEFINE_SPEC(FTestCharacterMovement)

using namespace ShootThemUp::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FFastRunLatentCommand, UInputComponent*, InputComponent);

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FNewJumpLatentCommand, UInputComponent*, InputComponent);
bool FNewJumpLatentCommand::Update()
{
    JumpPressed(InputComponent);
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

            It("Character jump with adecuate height",
                [this]()
                {
                    AActor* SUT_Character = Cast<AActor>(CreateBlueprintDeferred<ASTUBaseCharacter>(  //
                        World,                                                                      //
                        Character_bp,                                                               //
                        InitialTransform                                                            //
                    ));
                    // add 40 units for Z coordinate for

                    if (!TestNotNull("Character exist", SUT_Character)) return false;

                    SUT_Character->FinishSpawning(InitialTransform);

                    const FTransform TestItemsTransform{FVector{1300.0f, -1486.0f, 350.0f}};
                    ASTUTestItem* TestItem = CreateBlueprint<ASTUTestItem>(  //
                        World,                                               //
                        TestItemName,                                        //
                        TestItemsTransform);

                    const FTransform TestItemsTransformCantTaken{FVector{1300.0f, -1486.0f, 440.0f}};
                    ASTUTestItem* TestItemCantTaken = CreateBlueprint<ASTUTestItem>(  //
                        World,                                               //
                        TestItemName,                                        //
                        TestItemsTransformCantTaken);

                    if (!TestNotNull("TestItem exist", TestItem)) return false;
                    if (!TestNotNull("TestItem exist", TestItemCantTaken)) return false;

                    TArray<AActor*> TestItems;
                    UGameplayStatics::GetAllActorsOfClass(World, ASTUTestItem::StaticClass(), TestItems);
                    if (!TestEqual("Only one item exists", TestItems.Num(), 2)) return false;

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f))
                    ADD_LATENT_AUTOMATION_COMMAND(FNewJumpLatentCommand(SUT_Character->InputComponent));
                    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
                        [=]()
                        {
                            TArray<AActor*> TestItems;
                            UGameplayStatics::GetAllActorsOfClass(World, ASTUTestItem::StaticClass(), TestItems);
                            TestTrueExpr(TestItems.Num() == 1);
                            TestNotNull("Exist", TestItemCantTaken);

                        },
                        2.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });

            // AfterEach([this]() { SpecCloseLevel(); });
        });
}

#endif