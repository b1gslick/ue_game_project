// Shoot Them Up game, All right resievied

#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);

    if (!HealthComponent) return false;
    UE_LOG(LogTemp, Display, TEXT("Healthcomponent exist"));

    return HealthComponent->TryToAddHealth(HealthAmount);
}