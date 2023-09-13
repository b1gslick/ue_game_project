// This is My project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "ShootThemUp/Tests/TestUtils.h"
#include "Player/STUBaseCharacter.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "STUCoreTypes.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

BEGIN_DEFINE_SPEC(FTestCharacterHealthSystem, "ShootThemUp.HealthSystem",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

FHealthData HealthData;

END_DEFINE_SPEC(FTestCharacterHealthSystem)

using namespace ShootThemUp::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCheckHealth, ASTUBaseCharacter*, SUT_Character, float, ExceptedAmount);
bool FCheckHealth::Update()
{
    if (!FMath::IsNearlyEqual(SUT_Character->GetHealth(), ExceptedAmount)) return false;
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FCheckHealthComponent, AActor*, PlayerActor);
bool FCheckHealthComponent::Update()
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerActor);
    if (!HealthComponent) return false;
    return true;
}

void FTestCharacterHealthSystem::Define()
{

    Describe("Test Base health component",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("Map not exists", World);
                });
            It("Character should be hitted and health should be changed",
                [this]()
                {
                    ASTUBaseCharacter* SUT_Character = CreateBlueprint<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    const float ActualHealth = SUT_Character->GetHealth();
                    const float DamageAmount = ActualHealth / 99.9f;
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
                    const FTransform LandedInitialTransform{FVector{1280.0f, -1686.0f, 925.0f}};
                    ASTUBaseCharacter* SUT_Character =
                        CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp, LandedInitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    float Expected = HealthData.MaxHealth;
                    HealthData.MaxHealth = 100.0f;
                    CallFuncByNameWithParams(SUT_Character, "SetHealthData", {HealthData.ToString()});
                    SUT_Character->FinishSpawning(LandedInitialTransform);
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f))
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            TestTrueExpr(SUT_Character->GetHealth() < Expected);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });

            It("Character should be killed if he fell from a great height",
                [this]()
                {
                    const FTransform LandedInitialTransform{FVector{1280.0f, -1486.0f, 1200.0f}};
                    ASTUBaseCharacter* SUT_Character =
                        CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp, LandedInitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;
                    SUT_Character->FinishSpawning(LandedInitialTransform);
                    float Expected = 0.0f;
                    AActor* PlayerActor = Cast<AActor>(SUT_Character);
                    ADD_LATENT_AUTOMATION_COMMAND(FCheckHealthComponent(PlayerActor));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f))
                    ADD_LATENT_AUTOMATION_COMMAND(FCheckHealth(SUT_Character, Expected));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });

            It("Character should have auto heal when he doesn't take any damage",
                [this]()
                {
                    ASTUBaseCharacter* SUT_Character = CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;

                    HealthData.MaxHealth = 100.0f;
                    HealthData.AutoHeal = true;
                    HealthData.HealUpdateTime = 0.25f;
                    HealthData.HealDelay = 0.0f;
                    HealthData.HealModifier = 5.0f;

                    const float DamageAmount = HealthData.MaxHealth / 5.0f;

                    CallFuncByNameWithParams(SUT_Character, "SetHealthData", {HealthData.ToString()});
                    SUT_Character->FinishSpawning(InitialTransform);

                    SUT_Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

                    const float HealingDuration = HealthData.HealUpdateTime * DamageAmount / HealthData.HealModifier;
                    AActor* PlayerActor = Cast<AActor>(SUT_Character);
                    ADD_LATENT_AUTOMATION_COMMAND(FCheckHealthComponent(PlayerActor));
                    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
                        [=]()
                        {
                            if (!FMath::IsNearlyEqual(SUT_Character->GetHealth(), HealthData.MaxHealth))
                            {
                                UE_LOG(LogTemp, Error, TEXT("Health is not full, actual health %f"), SUT_Character->GetHealth());
                            }
                        },
                        HealingDuration));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
            It("Character has no autoheal if parameter for this is false", [this]() 
                {
                
                 ASTUBaseCharacter* SUT_Character = CreateBlueprintDeferred<ASTUBaseCharacter>(World, Character_bp, InitialTransform);
                    if (!TestNotNull("Character exist", SUT_Character)) return false;

                    HealthData.MaxHealth = 100.0f;
                    HealthData.AutoHeal = false;
                    HealthData.HealUpdateTime = 1.0f;
                    HealthData.HealDelay = 0.0f;
                    const float DamageAmount = HealthData.MaxHealth / 5.0f;

                    CallFuncByNameWithParams(SUT_Character, "SetHealthData", {HealthData.ToString()});
                    SUT_Character->FinishSpawning(InitialTransform);

                    SUT_Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

                    AActor* PlayerActor = Cast<AActor>(SUT_Character);
                    ADD_LATENT_AUTOMATION_COMMAND(FCheckHealthComponent(PlayerActor));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(HealthData.HealUpdateTime * 2.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FCheckHealth(SUT_Character, HealthData.MaxHealth - DamageAmount));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
            //AfterEach([this]() { SpecCloseLevel(); });
        });
}

#endif
