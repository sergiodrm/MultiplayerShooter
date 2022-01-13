#include "MultiplayerShooterLobbyWidget.h"

#include "Components/Button.h"

void UMultiplayerShooterLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    ReadyButton->OnClicked.AddDynamic(this, &UMultiplayerShooterLobbyWidget::OnReadyButtonPressed);
}

void UMultiplayerShooterLobbyWidget::OnReadyButtonPressed()
{
    // TODO
}
