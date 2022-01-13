#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooterTeam.generated.h"

UENUM()
enum class EMultiplayerShooterTeam : uint8
{
    None UMETA(Hidden),
    FirstTeam,
    SecondTeam,
    Count UMETA(Hidden)
};
