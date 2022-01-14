#include "MultiplayerShooterPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "MultiplayerShooter/Game/MultiplayerShooterGameState.h"
#include "Net/UnrealNetwork.h"

AMultiplayerShooterPlayerState::AMultiplayerShooterPlayerState()
{
    bUseCustomPlayerNames = false;
}

void AMultiplayerShooterPlayerState::OnRep_PlayerName()
{
    Super::OnRep_PlayerName();

    if (AMultiplayerShooterGameState* gameState = Cast<AMultiplayerShooterGameState>(UGameplayStatics::GetGameState(GetWorld())))
    {
        gameState->MulticastRPCHandleNewPlayerName(this);
    }
}
