#include "InteractableComponent.h"
#include "Components/PrimitiveComponent.h"

UInteractableComponent::UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::BeginPlay()
{
    Super::BeginPlay();
    if (bAutoEnableInteractionCollision)
    {
        if (AActor* Owner = GetOwner())
        {
            if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
            {
                // Prefer Interaction channel (GameTraceChannel1); fallback to Visibility if project hasn't set it up.
                Prim->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
                Prim->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
            }
        }
    }
}

bool UInteractableComponent::IsAvailable(AActor* Interactor) const
{
    if (MaxUsers > 0 && ActiveUsers >= MaxUsers) return false;
    if (bConsultOwnerImplementation && !bInOwnerCall && GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        TGuardValue<bool> Guard(bInOwnerCall, true);
        return IInteractable::Execute_CanInteract(GetOwner(), Interactor);
    }
    return true;
}

void UInteractableComponent::Begin(AActor* Interactor)
{
    if (!IsAvailable(Interactor)) return;
    ActiveUsers++;
    if (bConsultOwnerImplementation && !bInOwnerCall && GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        TGuardValue<bool> Guard(bInOwnerCall, true);
        IInteractable::Execute_BeginInteract(GetOwner(), Interactor);
    }
}

void UInteractableComponent::Complete(AActor* Interactor)
{
    if (ActiveUsers > 0) ActiveUsers--;
    if (bConsultOwnerImplementation && !bInOwnerCall && GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        TGuardValue<bool> Guard(bInOwnerCall, true);
        IInteractable::Execute_CompleteInteract(GetOwner(), Interactor);
    }
}

void UInteractableComponent::Cancel(AActor* Interactor)
{
    if (ActiveUsers > 0) ActiveUsers--;
    if (bConsultOwnerImplementation && !bInOwnerCall && GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        TGuardValue<bool> Guard(bInOwnerCall, true);
        IInteractable::Execute_CancelInteract(GetOwner(), Interactor);
    }
}
