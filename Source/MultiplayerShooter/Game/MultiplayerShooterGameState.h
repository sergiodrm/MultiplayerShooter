#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameState.h"

#include "MultiplayerShooterGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerNameListChanged, const TArray<APlayerState*>&);

UCLASS()
class AMultiplayerShooterGameState : public AGameState
{
    GENERATED_BODY()
public:
    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPCHandleNewPlayerName(class AMultiplayerShooterPlayerState* playerState);

public:
    FPlayerNameListChanged OnPlayerNameListChangedDelegate;
};
