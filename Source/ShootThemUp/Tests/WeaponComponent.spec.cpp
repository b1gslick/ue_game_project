// This is My project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Components/STUWeaponComponent.h"
#include "ShootThemUp/Tests/TestUtils.h"
#include "ShootThemUp/Public/Player/STUBaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "STUTestItem.h"
#include "STUUtils.h"
#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STURifleWeapon.h"
#include "GameFramework/Controller.h"

BEGIN_DEFINE_SPEC(FTestCharacterWeaponComponent, "ShootThemUp.Weapon.CharacterWeapon",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
static constexpr char* TestItemName = "/Script/Engine.Blueprint'/Game/Tests/BP_STUTestItem.BP_STUTestItem'";

END_DEFINE_SPEC(FTestCharacterWeaponComponent)

using namespace ShootThemUp::Test;

void FTestCharacterWeaponComponent::Define()
{
    Describe("Character can interact with weapon",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("Map not exists", World);
                });

            It("Character can shoot",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Character);
                    if (!WeaponComponent->GetCurrentWeapon()) return false;

                    FAmmoData BeforeAmmoData;

                    WeaponComponent->GetCurrentWeaponAmmoData(BeforeAmmoData);
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            WeaponComponent->StartFire();
                            return true;
                        }));

                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            FAmmoData AfterAmmoData;
                            WeaponComponent->GetCurrentWeaponAmmoData(AfterAmmoData);
                            TestTrueExpr((BeforeAmmoData.Bullets - AfterAmmoData.Bullets) == 1);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });

            It("If clips is full can't add another clips",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Character);
                    if (!WeaponComponent) return false;

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            WeaponComponent->NextWeapon();
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            const auto CurrentWeapon = WeaponComponent->GetCurrentWeapon();
                            TestTrueExpr(CurrentWeapon->IsA(ASTULauncherWeapon::StaticClass()));
                            TestTrueExpr(CurrentWeapon->TryToAddAmmo(1) == false);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
            It("We can add clips when ammo is not full",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Character);
                    if (!WeaponComponent) return false;

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            WeaponComponent->NextWeapon();
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            const auto CurrentWeapon = WeaponComponent->GetCurrentWeapon();
                            CurrentWeapon->StartFire();
                            TestTrueExpr(CurrentWeapon->IsAmmoFull() == false);
                            TestTrueExpr(CurrentWeapon->TryToAddAmmo(1) == true);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
            It("Rifle can zoom in",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Character);
                    if (!WeaponComponent) return false;

                    const auto Controller = Cast<APlayerController>(Character->Controller);
                    float AngleBefore = Controller->PlayerCameraManager->GetFOVAngle();

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            const auto CurrentWeapon = WeaponComponent->GetCurrentWeapon();
                            TestTrueExpr(CurrentWeapon->IsA(ASTURifleWeapon::StaticClass()));
                            WeaponComponent->Zoom(true);

                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            float AngleAfter = Controller->PlayerCameraManager->GetFOVAngle();
                            TestTrueExpr(FMath::IsNearlyEqual(AngleBefore, AngleAfter) == false);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
        });
}

#endif
