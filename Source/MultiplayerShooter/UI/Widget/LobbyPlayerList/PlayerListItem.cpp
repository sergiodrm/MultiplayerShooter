#include "PlayerListItem.h"

#include "GameFramework/PlayerState.h"
#include "MultiplayerShooter/Player/PlayerState/MultiplayerShooterPlayerState.h"

void UPlayerListItem::Init(const APlayerState* playerState)
{
    PlayerState = Cast<AMultiplayerShooterPlayerState>(playerState);
}
