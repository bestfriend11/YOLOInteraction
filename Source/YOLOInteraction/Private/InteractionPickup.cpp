#include "InteractionPickup.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableComponent.h"
#include "YIInventoryComponent.h"
#include "YIItemDefinition.h"
#include "YOLOInteraction.h"

AInteractionPickup::AInteractionPickup()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    Interactable = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
    Interactable->Prompt = NSLOCTEXT("YOLOInteraction", "PickupPrompt", "Pick Up");
}

bool AInteractionPickup::CanInteract_Implementation(AActor* Interactor) const
{
    return Interactable ? Interactable->IsAvailable(Interactor) : false;
}

void AInteractionPickup::BeginInteract_Implementation(AActor* Interactor)
{
    if (Interactable) Interactable->Begin(Interactor);
}

void AInteractionPickup::CompleteInteract_Implementation(AActor* Interactor)
{
    if (GiveTo(Interactor))
    {
        if (bDestroyOnPickup) Destroy();
    }
    if (Interactable) Interactable->Complete(Interactor);
}

FText AInteractionPickup::GetInteractionPrompt_Implementation() const
{
    return Interactable ? Interactable->Prompt : FText::GetEmpty();
}

bool AInteractionPickup::GiveTo(AActor* Interactor)
{
    if (!Interactor || ItemDef.IsNull()) return false;
    UYIInventoryComponent* Inv = Interactor->FindComponentByClass<UYIInventoryComponent>();
    if (!Inv || !Inv->GetBag()) return false;

    FYIItemInstance ItemInstance;
    ItemInstance.Definition = ItemDef;
    ItemInstance.Count = Count;
    FYIBagItem BagItem;
    BagItem.Item = ItemInstance;
    BagItem.Size = ItemDef.IsValid() ? ItemDef.Get()->DefaultSize : FIntPoint(1,1);
    const int32 Added = Inv->GetBag()->AddBagItem(BagItem);
    if (Added != INDEX_NONE)
    {
        if (GYOLOInteractionDebug && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Cyan,
                FString::Printf(TEXT("[Pickup] %s x%d -> %s"), *ItemDef.ToString(), Count, *GetNameSafe(Interactor)));
        }
        Inv->SyncNetState();
        return true;
    }
    return false;
}
