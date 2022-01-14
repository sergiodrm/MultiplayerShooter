#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "MultiplayerShooterPlayerState.generated.h"

UCLASS()
class AMultiplayerShooterPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    AMultiplayerShooterPlayerState();

    virtual void OnRep_PlayerName() override;
};
