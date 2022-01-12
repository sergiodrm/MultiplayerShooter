// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MultiplayerShooterTeam.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerShooterGameMode.generated.h"

USTRUCT()
struct FMultiplayerTeam
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    TArray<class APlayerController*> TeamControllers;
};

UCLASS(minimalapi)
class AMultiplayerShooterGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMultiplayerShooterGameMode();
protected:
    virtual void BeginPlay() override;
public:
    virtual void PostLogin(APlayerController* NewPlayer) override;

    UFUNCTION(BlueprintCallable)
    FColor GetTeamColor(EMultiplayerShooterTeam team) const;

protected:
    EMultiplayerShooterTeam LoginNewController(APlayerController* newController);
    class AMultiplayerShooterCharacter* SpawnCharacter(const class ATargetPoint* spawnPoint) const;
    class ATargetPoint* GetRandomSpawnPoint() const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Players")
    TMap<EMultiplayerShooterTeam, FColor> TeamColors;
    UPROPERTY(EditDefaultsOnly, Category = "Players")
    TSubclassOf<AMultiplayerShooterCharacter> DefaultCharacterClass;

    UPROPERTY()
    TArray<class ATargetPoint*> SpawnPoints;

    UPROPERTY()
    TMap<EMultiplayerShooterTeam, FMultiplayerTeam> Teams;
};
