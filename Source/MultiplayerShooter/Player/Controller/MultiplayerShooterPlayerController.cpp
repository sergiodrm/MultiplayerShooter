#include "MultiplayerShooterPlayerController.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerShooter/Game/MultiplayerShooterGameMode.h"

AMultiplayerShooterPlayerController::AMultiplayerShooterPlayerController()
{}

void AMultiplayerShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    const FInputModeUIOnly inputMode;
    SetInputMode(inputMode);
    bShowMouseCursor = true;

    const TActorIterator<ACameraActor> it(GetWorld());
    SetViewTarget(*it);
}

void AMultiplayerShooterPlayerController::ServerRPCLoginPlayerName_Implementation(const FString& newName)
{
    bool success = false;
    if (!newName.IsEmpty())
    {
        if (AMultiplayerShooterGameMode* gameMode = Cast<AMultiplayerShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
        {
            success = gameMode->LoginNewPlayer(newName, this);
        }
    }
    ClientRPCConfirmLoginRequest(success);
}

void AMultiplayerShooterPlayerController::ClientRPCConfirmLoginRequest_Implementation(bool success)
{
    OnLoginResultReceivedDelegate.ExecuteIfBound(success);
}
