// Shoot Them Up game, All right resievied

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;

USTRUCT(BlueprintType)
struct FHitDamagePoints
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
    float Head{50.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
    float Other{10.0f};

    FString ToString() const
    {                                                       //
        return FString::Printf(TEXT("(Head=%f,Other=%f)"),  //
            Head,                                           //
            Other                                           //
        );
    }
};

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    virtual void BeginPlay() override;
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageAmountByHitPoint")
    FHitDamagePoints HitDamagePoints;

private:
    FTimerHandle ShotTimerHandle;

    void MakeDamage(const FHitResult& HitResult);
};
