#include "MultiplayerShooterHUD.h"

#include "Widget/MultiplayerShooterLobbyWidget.h"

AMultiplayerShooterHUD::AMultiplayerShooterHUD()
{
    LobbyWidgetClass = UMultiplayerShooterLobbyWidget::StaticClass();
}

void AMultiplayerShooterHUD::BeginPlay()
{
    Super::BeginPlay();

    if (LobbyWidgetClass != nullptr)
    {
        LobbyWidget = CreateWidget<UMultiplayerShooterLobbyWidget>(GetWorld(), LobbyWidgetClass);
        if (LobbyWidget)
        {
            LobbyWidget->AddToViewport();
        }
    }
}
