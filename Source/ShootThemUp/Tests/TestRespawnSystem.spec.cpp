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
#include "GameFramework/PlayerController.h"
#include "Components/STURespawnComponent.h"
#include "Player/STUPlayerController.h"

BEGIN_DEFINE_SPEC(FTestRespawnSystem, "ShootThemUp.RespawnSystem",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

END_DEFINE_SPEC(FTestRespawnSystem)

using namespace ShootThemUp::Test;

void FTestRespawnSystem::Define()
{

    Describe("Test respawn system after death",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("Map not exists", World);
                });
            It("Character should be respawned after killed",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                    ASTUPlayerController* PlayerController = Cast<ASTUPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
                    const float DamageAmount = Character->GetHealth();
                    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(PlayerController);

                    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
                    TestTrueExpr(RespawnComponent->IsRespawnInProgress() == true);
                    float RespawnCountDown = float(RespawnComponent->GetRespawnCountDown() + 0.5f);
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(RespawnCountDown));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            ASTUBaseCharacter* NewCharacter = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                            TestTrueExpr(NewCharacter->GetHealthPercents() == 1.0f);
                            TestTrueExpr(RespawnComponent->IsRespawnInProgress() == false);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });

            It("When character is Alive respawn time is 0", [this]() {
                    ASTUPlayerController* PlayerController = Cast<ASTUPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
                    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(PlayerController);
                    TestTrueExpr(RespawnComponent->IsRespawnInProgress() == false);
                    TestTrueExpr(RespawnComponent->GetRespawnCountDown() == 0.0f);
                });
        });
}

#endif
