// This is My project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ShootThemUp/Tests/TestUtils.h"
#include "Player/STUBaseCharacter.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

// DEFINE_SPEC(FTestClassName, "Category.TestName",
//    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

BEGIN_DEFINE_SPEC(FTestCharacterHealthSystem, "ShootThemUp.HealthSystem",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
END_DEFINE_SPEC(FTestCharacterHealthSystem)

using namespace ShootThemUp::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, UInputComponent*, InputComponent);
bool FJumpLatentCommand::Update()
{
    JumpPressed(InputComponent);
    return true;
}

// FString TheFloatStr = FString::SanitizeFloat(f);

namespace
{
constexpr char* MapName = "/Game/Levels/TestLevel";
constexpr char* Character_bp = "/Script/Engine.Blueprint'/Game/Player/BP_STUBaseCharacter.BP_STUBaseCharacter'";
const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
}  // namespace

void FTestCharacterHealthSystem::Define()
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
            It("After spawn character should have not 0 health",
                [this]()
                {
                    ASTUBaseCharacter* SUT_Character = CreateBlueprint<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exists", SUT_Character)) return false;

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f))
                    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(SUT_Character->InputComponent));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f))

                    TestTrueExpr(SUT_Character->GetHealth() != 0);
                    return true;
                });
            It("Character should be hitted and health should be changed",
                [this]()
                {
                    ASTUBaseCharacter* SUT_Character = CreateBlueprint<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    const float ActualHealth = SUT_Character->GetHealth();
                    const float DamageAmount = ActualHealth / 99.9f;
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f))
                    SUT_Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
                    TestTrueExpr(SUT_Character->GetHealth() == ActualHealth - DamageAmount);

                    return true;
                });

            It("Character should be killed",
                [this]()
                {
                    ASTUBaseCharacter* SUT_Character = CreateBlueprint<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    const float ActualHealth = SUT_Character->GetHealth();

                    SUT_Character->TakeDamage(ActualHealth, FDamageEvent{}, nullptr, nullptr);
                    TestTrueExpr(SUT_Character->GetHealth() == 0);

                    return true;
                });
            It("Character should be get hitted when landed",
                [this]()
                {
                    const FTransform LandedInitialTransform{FVector{1200.0f, -600.0f, 625.0f}};
                    ASTUBaseCharacter* SUT_Character = CreateBlueprint<ASTUBaseCharacter>(World, Character_bp, LandedInitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    // SUT_Character->FinishSpawning(LandedInitialTransform);
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f))
                    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(SUT_Character->InputComponent));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));
                    UE_LOG(LogTemp, Display, TEXT("Currnet health is: %f"), SUT_Character->GetHealth());
                    TestTrueExpr(SUT_Character->GetHealth() == 0);

                    return true;
                });

            It("Character should be killed if he fell from a great height",
                [this]()
                {
                    const FTransform LandedInitialTransform{FVector{1200.0f, -600.0f, 1200.0f}};
                    ASTUBaseCharacter* SUT_Character =
                        CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp, LandedInitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    SUT_Character->FinishSpawning(LandedInitialTransform);
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f))
                    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(SUT_Character->InputComponent));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));
                    UE_LOG(LogTemp, Display, TEXT("Currnet health is: %f"), SUT_Character->GetHealth());
                    TestTrueExpr(SUT_Character->GetHealth() == 0);

                    return true;
                });

            It("Character should have auto heal when he doens't take any damage",
                [this]()
                {
                    ASTUBaseCharacter* SUT_Character = CreateBlueprint<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    const float StartedHealth = SUT_Character->GetHealth();
                    const float DamageAmount = StartedHealth / 5.0f;

                    SUT_Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

                    const float MaxHealth = 100.0f;
                    const float HealModifier = 1.0f;
                    const float HealRate = 0.3f;

                    TestTrueExpr(SUT_Character->GetHealth() == StartedHealth - DamageAmount);

                    const float HealthDiff = MaxHealth - SUT_Character->GetHealth();
                    const float HealingDuration = HealRate * HealthDiff / HealModifier;

                    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
                        [SUT_Character, StartedHealth]()
                        {
                            if (!FMath::IsNearlyEqual(SUT_Character->GetHealth(), StartedHealth))
                            {
                                UE_LOG(LogTemp, Error, TEXT("Health is not full"));
                            }
                        },
                        HealingDuration));
                    return true;
                });
        });
    // AfterEach([this]() { SpecCloseLevel(World); });
}

#endif
