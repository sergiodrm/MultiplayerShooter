#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooterPlayerController.generated.h"

DECLARE_DELEGATE_OneParam(FLoginResultReceived, bool);

UCLASS()
class AMultiplayerShooterPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    AMultiplayerShooterPlayerController();

protected:
    virtual void BeginPlay() override;
public:
    /**
     * @brief Server RPC to register the new name of the player in game mode.
     * This will check if the name is valid and will notify with the result.
     * @param newName the new name of the player.
    */
    UFUNCTION(Server, Reliable)
    void ServerRPCLoginPlayerName(const FString& newName);
    /**
     * @brief Client RPC will be called from server when it tries to change the player's name.
     * @param success true if the name was changed successfully, false otherwise.
    */
    UFUNCTION(Client, Reliable)
    void ClientRPCConfirmLoginRequest(bool success);

public:
    FLoginResultReceived OnLoginResultReceivedDelegate;

protected:
};
