#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "MultiplayerShooterLobbyWidget.generated.h"

UCLASS()
class UMultiplayerShooterLobbyWidget : public UUserWidget
{
    GENERATED_BODY()
public:
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnReadyButtonPressed();

    UFUNCTION()
    void OnPlayerNameListChanged(const TArray<APlayerState*>& playerList);

private:
    UPROPERTY(meta = (BindWidget))
    class UPlayerListPanelWidget* PlayerListPanel;
    UPROPERTY(meta = (BindWidget))
    class UButton* ReadyButton;
};
