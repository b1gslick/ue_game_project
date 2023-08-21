// Shoot Them Up game, All right resievied

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SUTHealthPercentDecorator.generated.h"

UCLASS()
class SHOOTTHEMUP_API USUTHealthPercentDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    USUTHealthPercentDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.6f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
