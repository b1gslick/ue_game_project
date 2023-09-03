// Shoot Them Up game, All right resievied


#include "Menu/UI/STUGoToMainMenuWidget.h"
#include "STUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMainMenuWidget, All, All);


void USTUGoToMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GoToMainMenuButton)
    {
        GoToMainMenuButton->OnClicked.AddDynamic(this, &USTUGoToMainMenuWidget::OnGoToMainMenu);
    }
}

void USTUGoToMainMenuWidget::OnGoToMainMenu()
{
    if (!GetWorld()) return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (!STUGameInstance) return;

    if (STUGameInstance->GetMainMenuName().IsNone())
    {
        UE_LOG(LogSTUGoToMainMenuWidget, Error, TEXT("Menu level name is NONE"));
        return;
    }

    const FName StartUpLevelName = "TestLevel";
    UGameplayStatics::OpenLevel(this, STUGameInstance->GetMainMenuName());
}