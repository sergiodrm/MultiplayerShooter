#include "MultiplayerShooterLobbyWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPlayerList/PlayerListPanelWidget.h"
#include "MultiplayerShooter/Game/MultiplayerShooterGameState.h"

void UMultiplayerShooterLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    ReadyButton->OnClicked.AddDynamic(this, &UMultiplayerShooterLobbyWidget::OnReadyButtonPressed);

    AMultiplayerShooterGameState* gameState = Cast<AMultiplayerShooterGameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (gameState)
    {
        gameState->OnPlayerNameListChangedDelegate.AddUObject(this, &UMultiplayerShooterLobbyWidget::OnPlayerNameListChanged);
    }
}

void UMultiplayerShooterLobbyWidget::OnReadyButtonPressed()
{
    // TODO
}

void UMultiplayerShooterLobbyWidget::OnPlayerNameListChanged(const TArray<APlayerState*>& playerList)
{
    for (const APlayerState* playerState : playerList)
    {
        PlayerListPanel->AddEntry(playerState);
    }
}
