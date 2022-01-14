#include "MultiplayerShooterGameState.h"

#include "GameFramework/PlayerState.h"

void AMultiplayerShooterGameState::MulticastRPCHandleNewPlayerName_Implementation(AMultiplayerShooterPlayerState* playerState)
{
    OnPlayerNameListChangedDelegate.Broadcast(PlayerArray);
}
