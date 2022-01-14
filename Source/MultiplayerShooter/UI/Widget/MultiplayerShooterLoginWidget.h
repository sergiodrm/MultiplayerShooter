#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "MultiplayerShooterLoginWidget.generated.h"

UCLASS()
class UMultiplayerShooterLoginWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    void HandleInvalidName();

protected:
    UFUNCTION()
    void OnConfirmButtonPressed();

private:
    UPROPERTY(Transient, meta = (BindWidget))
    class UEditableTextBox* InputNameWidget;
    UPROPERTY(Transient, meta = (BindWidget))
    class UButton* ConfirmButton;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    class UWidgetAnimation* WaitingAnimation;
};
