// This is My project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Helpers/TestUtils.h"
#include "Player/STUBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "STUCoreTypes.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Pickups/STUAmmoPickup.h"
#include "Pickups/STUHealthPickup.h"
#include "Weapon/STUBaseWeapon.h"

BEGIN_DEFINE_SPEC(FTestPickUps, "ShootThemUp.PickUps",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

FHealthData HealthData;

FString HealthPickUp = "/Script/Engine.Blueprint'/Game/Pickups/BP_STUHealthPickup.BP_STUHealthPickup'";
FString AmmoPickUp = "/Script/Engine.Blueprint'/Game/Pickups/BP_STUAmmoPickup.BP_STUAmmoPickup'";

END_DEFINE_SPEC(FTestPickUps)

using namespace ShootThemUp::Test;


void FTestPickUps::Define()
{
    Describe("Test Pickups",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("Map not exists", World);
                });
            It("Health Pick ups can't be taken if health is full",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                    const auto Transform = FTransform{Character->GetActorLocation()};
                    ASTUHealthPickup* PickUP = CreateBlueprint<ASTUHealthPickup>(  //
                        World,                                                     //
                        HealthPickUp,                                              //
                        Transform);

                    TArray<AActor*> HealthPickUp;
                    UGameplayStatics::GetAllActorsOfClass(World, ASTUHealthPickup::StaticClass(), HealthPickUp);
                    TestTrueExpr(HealthPickUp.Num() == 1);
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
            It("Health Pick ups can be taken if health is not full",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            const float ActualHealth = Character->GetHealth();
                            const float DamageAmount = ActualHealth - 0.1f;
                            Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            const auto Transform = FTransform{Character->GetActorLocation()};
                            ASTUHealthPickup* PickUP = CreateBlueprint<ASTUHealthPickup>(  //
                                World,                                                     //
                                HealthPickUp,                                              //
                                Transform);
                            TestTrueExpr(PickUP->CouldBeTaken() == false);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
            It("Ammo Pick ups can't be taken if ammo is full",
                [this]()
                {
                    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
                    const auto Transform = FTransform{Character->GetActorLocation()};

                    ASTUAmmoPickup* AmmoPickUP = CreateBlueprint<ASTUAmmoPickup>(  //
                        World,                                                     //
                        AmmoPickUp,                                                //
                        Transform);

                    TArray<AActor*> AmmoPickUp;
                    UGameplayStatics::GetAllActorsOfClass(World, ASTUAmmoPickup::StaticClass(), AmmoPickUp);
                    TestTrueExpr(AmmoPickUp.Num() == 1);
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
            It("Ammo Pick ups can be taken if ammo is not full",
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
                            WeaponComponent->Reload();
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [=]()
                        {
                            const auto Transform = FTransform{Character->GetActorLocation()};

                            ASTUAmmoPickup* AmmoPickUP = CreateBlueprint<ASTUAmmoPickup>(  //
                                World,                                                     //
                                AmmoPickUp,                                                //
                                Transform);
                            TestTrueExpr(AmmoPickUP->CouldBeTaken() == false);
                            return true;
                        }));
                    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                    return true;
                });
        });
}

#endif
