#pragma once

#include "CoreMinimal.h"

#include "GameFramework/HUD.h"

#include "MultiplayerShooterHUD.generated.h"

UCLASS()
class AMultiplayerShooterHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMultiplayerShooterHUD();

    virtual void BeginPlay() override;

protected:
    UFUNCTION()
    void OnPlayerControllerLogin(bool success);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Menu", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UMultiplayerShooterLoginWidget> LoginWidgetClass;
    UPROPERTY(Transient)
    class UMultiplayerShooterLoginWidget* LoginWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Menu", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UMultiplayerShooterLobbyWidget> LobbyWidgetClass;
    UPROPERTY(Transient)
    class UMultiplayerShooterLobbyWidget* LobbyWidget;
};
