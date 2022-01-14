#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

#include "PlayerListEntryWidget.generated.h"

UCLASS(Abstract)
class UPlayerListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()
public:
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* NameLabel;
    UPROPERTY(meta = (BindWidget))
    class UImage* IconImage;
};
