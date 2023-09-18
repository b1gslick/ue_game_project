// Shoot Them Up game, All right resievied

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUTestItem.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUTestItem : public AActor
{
    GENERATED_BODY()

public:
    ASTUTestItem();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;
};
