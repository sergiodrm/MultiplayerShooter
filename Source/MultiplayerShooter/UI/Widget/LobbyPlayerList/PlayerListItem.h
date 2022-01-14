#pragma once

#include "CoreMinimal.h"
#include "PlayerListItem.generated.h"

UCLASS()
class UPlayerListItem : public UObject
{
    GENERATED_BODY()
public:
    void Init(const APlayerState* playerState);

    bool Equals(const UPlayerListItem* other) const { return PlayerState == other->PlayerState; }

    UPROPERTY()
    const class AMultiplayerShooterPlayerState* PlayerState;
};
