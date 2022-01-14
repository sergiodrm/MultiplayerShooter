#include "PlayerListPanelWidget.h"

#include "PlayerListItem.h"
#include "Components/ListView.h"

void UPlayerListPanelWidget::NativeConstruct()
{ }

void UPlayerListPanelWidget::AddEntry(const APlayerState* playerState)
{
    if (playerState)
    {
        UPlayerListItem* newItem = NewObject<UPlayerListItem>(this);
        newItem->Init(playerState);

        if (UPlayerListItem* oldItem = FindItem(newItem))
        {
            UpdateItem(oldItem, newItem);
        }
        else
        {
            PlayerListView->AddItem(newItem);
        }
    }
}

void UPlayerListPanelWidget::UpdateItem(UPlayerListItem* oldItem, UPlayerListItem* newItem)
{
    PlayerListView->RemoveItem(oldItem);
    PlayerListView->AddItem(newItem);
}

UPlayerListItem* UPlayerListPanelWidget::FindItem(const UPlayerListItem* item) const
{
    UPlayerListItem* oldItem = nullptr;
    for (UObject* object : PlayerListView->GetListItems())
    {
        if (UPlayerListItem* currentItem = Cast<UPlayerListItem>(object))
        {
            if (currentItem->Equals(item))
            {
                oldItem = currentItem;
                break;
            }
        }
    }
    return oldItem;
}
