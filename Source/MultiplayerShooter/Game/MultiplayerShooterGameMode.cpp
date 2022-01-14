// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerShooterGameMode.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "MultiplayerShooter/Player/Character/MultiplayerShooterCharacter.h"
#include "Engine/TargetPoint.h"
#include "MultiplayerShooter/Player/Controller/MultiplayerShooterPlayerController.h"
#include "MultiplayerShooter/Player/PlayerState/MultiplayerShooterPlayerState.h"
#include "MultiplayerShooter/UI/MultiplayerShooterHUD.h"

AMultiplayerShooterGameMode::AMultiplayerShooterGameMode()
{
    DefaultPawnClass = nullptr;
    DefaultCharacterClass = AMultiplayerShooterCharacter::StaticClass();
    PlayerStateClass = AMultiplayerShooterPlayerState::StaticClass();
    HUDClass = AMultiplayerShooterHUD::StaticClass();

    // Create the teams
    for (uint8 team = static_cast<uint8>(EMultiplayerShooterTeam::FirstTeam); team < static_cast<uint8>(EMultiplayerShooterTeam::Count); ++team)
    {
        Teams.Add(static_cast<EMultiplayerShooterTeam>(team), FMultiplayerTeam {});
    }
}

void AMultiplayerShooterGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Get all the spawn points in level
    for (TActorIterator<ATargetPoint> it(GetWorld()); it; ++it)
    {
        SpawnPoints.Add(*it);
    }

    // Spawn a character for each controller
    //for (TMap<EMultiplayerShooterTeam, FMultiplayerTeam>::TIterator it(Teams); it; ++it)
    //{
    //    for (APlayerController* controller : it->Value.TeamControllers)
    //    {
    //        const ATargetPoint* spawnPoint = GetRandomSpawnPoint();
    //        AMultiplayerShooterCharacter* character = SpawnCharacter(spawnPoint);
    //        controller->Possess(character);
    //    }
    //}
}

void AMultiplayerShooterGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    LoginNewController(NewPlayer);

    // Placeholder
    if (!SpectatorCamera)
    {
        for (TActorIterator<ACameraActor> it(GetWorld()); it; ++it)
        {
            SpectatorCamera = *it;
            break;
        }
    }

    NewPlayer->SetViewTarget(SpectatorCamera);
}

bool AMultiplayerShooterGameMode::LoginNewPlayer(const FString& playerName, AMultiplayerShooterPlayerController* newController)
{
    const bool isValid = IsPlayerNameValid(playerName);
    if (isValid)
    {
        AMultiplayerShooterPlayerState* playerState = newController->GetPlayerState<AMultiplayerShooterPlayerState>();
        playerState->SetPlayerName(playerName);
    }
    return isValid;
}

FColor AMultiplayerShooterGameMode::GetTeamColor(EMultiplayerShooterTeam team) const
{
    return TeamColors.Contains(team) ? TeamColors[team] : FColor::White;
}

EMultiplayerShooterTeam AMultiplayerShooterGameMode::LoginNewController(APlayerController* newController)
{
    EMultiplayerShooterTeam team = EMultiplayerShooterTeam::None;
    uint32 minTeamMembers = UINT32_MAX;
    for (const TPair<EMultiplayerShooterTeam, FMultiplayerTeam>& it : Teams)
    {
        const uint32 members = it.Value.TeamControllers.Num();
        if (members < minTeamMembers)
        {
            minTeamMembers = members;
            team = it.Key;
        }
    }
    Teams[team].TeamControllers.Add(newController);
    return team;
}

AMultiplayerShooterCharacter* AMultiplayerShooterGameMode::SpawnCharacter(const ATargetPoint* spawnPoint) const
{
    AMultiplayerShooterCharacter* character = nullptr;
    if (spawnPoint)
    {
        const FVector spawnLocation = spawnPoint->GetActorLocation();
        const FRotator spawnRotation = spawnPoint->GetActorRotation();
        const FActorSpawnParameters spawnParameters;
        character = GetWorld()->SpawnActor<AMultiplayerShooterCharacter>(DefaultCharacterClass.Get(), spawnLocation, spawnRotation, spawnParameters);
    }
    return character;
}

ATargetPoint* AMultiplayerShooterGameMode::GetRandomSpawnPoint() const
{
    const uint32 index = static_cast<uint32>(FMath::RandRange(1, SpawnPoints.Num()) - 1);
    return SpawnPoints[index];
}

bool AMultiplayerShooterGameMode::IsPlayerNameValid(const FString& playerName) const
{
    bool valid = true;
    for (const auto& team : Teams)
    {
        for (APlayerController* controller : team.Value.TeamControllers)
        {
            if (controller)
            {
                const APlayerState* playerState = controller->GetPlayerState<APlayerState>();
                const FString otherName = playerState->GetPlayerNameCustom();
                if (otherName.Equals(playerName))
                {
                    valid = false;
                    break;
                }
            }
        }
    }
    return valid;
}
