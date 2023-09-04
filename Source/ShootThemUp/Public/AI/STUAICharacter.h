// Shoot Them Up game, All right resievied

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

public:
    ASTUAICharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    virtual void OnDeath() override;
    virtual void OnHealthChanged(float Health, float HealthDelta) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthVisibilityDistance = 1000.0f;

private:
    void UpdateHealthWidgetVisibility();
};
