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

private:
    UPROPERTY(EditDefaultsOnly, Category = "Menu", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UMultiplayerShooterLobbyWidget> LobbyWidgetClass;
    UPROPERTY(Transient)
    class UMultiplayerShooterLobbyWidget* LobbyWidget;
};
