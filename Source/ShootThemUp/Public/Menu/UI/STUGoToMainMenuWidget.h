// Shoot Them Up game, All right resievied

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGoToMainMenuWidget.generated.h"

class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUGoToMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* GoToMainMenuButton;
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnGoToMainMenu();
};
