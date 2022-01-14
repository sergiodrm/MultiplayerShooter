#include "MultiplayerShooterHUD.h"

#include "Components/TextBlock.h"
#include "MultiplayerShooter/Player/Controller/MultiplayerShooterPlayerController.h"
#include "Widget/MultiplayerShooterLobbyWidget.h"
#include "Widget/MultiplayerShooterLoginWidget.h"

AMultiplayerShooterHUD::AMultiplayerShooterHUD()
{
    LobbyWidgetClass = UMultiplayerShooterLobbyWidget::StaticClass();
}

void AMultiplayerShooterHUD::BeginPlay()
{
    Super::BeginPlay();


    if (AMultiplayerShooterPlayerController* controller = Cast<AMultiplayerShooterPlayerController>(GetOwningPlayerController()))
    {
        controller->OnLoginResultReceivedDelegate.BindUObject(this, &AMultiplayerShooterHUD::OnPlayerControllerLogin);
    }

    if (LoginWidgetClass != nullptr)
    {
        LoginWidget = CreateWidget<UMultiplayerShooterLoginWidget>(GetWorld(), LoginWidgetClass);
        LoginWidget->AddToViewport();
    }
    if (LobbyWidgetClass != nullptr)
    {
        LobbyWidget = CreateWidget<UMultiplayerShooterLobbyWidget>(GetWorld(), LobbyWidgetClass);
    }
}

void AMultiplayerShooterHUD::OnPlayerControllerLogin(bool success)
{
    if (success)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Green, TEXT("Player name logged correctly!"));
        LoginWidget->DebugText->SetText(FText::FromString(TEXT("Player name logged correctly!")));
        LoginWidget->DebugText->SetColorAndOpacity(FSlateColor(FColor::Green));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, TEXT("Invalid player name!"));
        LoginWidget->DebugText->SetText(FText::FromString(TEXT("Invalid player name!")));
        LoginWidget->DebugText->SetColorAndOpacity(FSlateColor(FColor::Red));
    }
}
