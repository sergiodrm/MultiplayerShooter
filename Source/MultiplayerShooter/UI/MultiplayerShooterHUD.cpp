#include "MultiplayerShooterHUD.h"

#include "MultiplayerShooter/Player/Controller/MultiplayerShooterPlayerController.h"
#include "Widget/MultiplayerShooterLobbyWidget.h"
#include "Widget/MultiplayerShooterLoginWidget.h"

AMultiplayerShooterHUD::AMultiplayerShooterHUD()
{
    LobbyWidgetClass = UMultiplayerShooterLobbyWidget::StaticClass();
}

void AMultiplayerShooterHUD::BeginPlay()
{
    Super::BeginPlay();

    if (AMultiplayerShooterPlayerController* controller = Cast<AMultiplayerShooterPlayerController>(GetOwningPlayerController()))
    {
        controller->OnLoginResultReceivedDelegate.BindUObject(this, &AMultiplayerShooterHUD::OnPlayerControllerLogin);
    }

    if (LoginWidgetClass != nullptr)
    {
        LoginWidget = CreateWidget<UMultiplayerShooterLoginWidget>(GetWorld(), LoginWidgetClass);
        LoginWidget->AddToViewport();
    }
    if (LobbyWidgetClass != nullptr)
    {
        LobbyWidget = CreateWidget<UMultiplayerShooterLobbyWidget>(GetWorld(), LobbyWidgetClass);
    }
}

void AMultiplayerShooterHUD::OnPlayerControllerLogin(bool success)
{
    if (success)
    {
        LoginWidget->RemoveFromViewport();
        LobbyWidget->AddToViewport();
    }
    else
    {
        LoginWidget->HandleInvalidName();
    }
}
