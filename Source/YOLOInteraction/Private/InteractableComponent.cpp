#include "InteractableComponent.h"

UInteractableComponent::UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UInteractableComponent::IsAvailable(AActor* Interactor) const
{
    if (MaxUsers > 0 && ActiveUsers >= MaxUsers) return false;
    if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        return IInteractable::Execute_CanInteract(GetOwner(), Interactor);
    }
    return true;
}

void UInteractableComponent::Begin(AActor* Interactor)
{
    if (!IsAvailable(Interactor)) return;
    ActiveUsers++;
    if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        IInteractable::Execute_BeginInteract(GetOwner(), Interactor);
    }
}

void UInteractableComponent::Complete(AActor* Interactor)
{
    if (ActiveUsers > 0) ActiveUsers--;
    if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        IInteractable::Execute_CompleteInteract(GetOwner(), Interactor);
    }
}

void UInteractableComponent::Cancel(AActor* Interactor)
{
    if (ActiveUsers > 0) ActiveUsers--;
    if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        IInteractable::Execute_CancelInteract(GetOwner(), Interactor);
    }
}
