#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "PlayerListPanelWidget.generated.h"

UCLASS(Abstract)
class UPlayerListPanelWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    void AddEntry(const APlayerState* playerState);

protected:
    void UpdateItem(class UPlayerListItem* oldItem, class UPlayerListItem* newItem);
    class UPlayerListItem* FindItem(const UPlayerListItem* item) const;

private:
    UPROPERTY(meta = (BindWidget))
    class UListView* PlayerListView;
};
