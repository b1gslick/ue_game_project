// Shoot Them Up game, All right resievied

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClapMin = "1.0", ClapMax = "100.0"))
    float HealthAmount = 50.0f;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
