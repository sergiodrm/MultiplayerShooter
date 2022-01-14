#include "MultiplayerShooterLoginWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "MultiplayerShooter/Player/Controller/MultiplayerShooterPlayerController.h"

void UMultiplayerShooterLoginWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ConfirmButton->OnClicked.AddDynamic(this, &UMultiplayerShooterLoginWidget::OnConfirmButtonPressed);
}

void UMultiplayerShooterLoginWidget::OnConfirmButtonPressed()
{
    AMultiplayerShooterPlayerController* playerController = GetOwningPlayer<AMultiplayerShooterPlayerController>();
    if (playerController)
    {
        // Notify player controller the new name
        const FString playerName = InputNameWidget->GetText().ToString();
        playerController->ServerRPCLoginPlayerName(playerName);

        // Play waiting animation to give feedback to the player
        PlayAnimation(WaitingAnimation, 0.f, 0);
    }
}
