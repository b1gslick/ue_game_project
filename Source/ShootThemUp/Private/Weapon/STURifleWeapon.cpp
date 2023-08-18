// Shoot Them Up game, All right resievied

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }
    FVector TraceStart, TraceEnd;

    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        MakeDamage(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    FString HeadBone = "b_Head";

    AActor* Target = HitResult.GetActor();
    if (!Target) return;
    const auto Player = Cast<ACharacter>(GetOwner());

    if (HitResult.BoneName.ToString() == HeadBone)
    {
        Target->TakeDamage(HitDamagePoints.Head, FDamageEvent{}, GetPlayerController(), this);
    }
    else
    {
        Target->TakeDamage(HitDamagePoints.Other, FDamageEvent{}, GetPlayerController(), this);
    }
}