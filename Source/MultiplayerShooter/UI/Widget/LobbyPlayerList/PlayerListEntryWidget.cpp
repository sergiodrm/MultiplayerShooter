#include "PlayerListEntryWidget.h"

#include "PlayerListItem.h"
#include "Components/TextBlock.h"
#include "MultiplayerShooter/Player/PlayerState/MultiplayerShooterPlayerState.h"

void UPlayerListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (UPlayerListItem* item = Cast<UPlayerListItem>(ListItemObject))
    {
        const FString name = item->PlayerState->GetPlayerNameCustom();
        NameLabel->SetText(FText::FromString(name));
    }
}
