#include "InteractionComponent.h"
#include "InteractableComponent.h"
#include "InteractionInterfaces.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(false);
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateFocus();
}

void UInteractionComponent::UpdateFocus()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;
    FVector Start, Dir;
    if (APawn* Pawn = Cast<APawn>(Owner))
    {
        if (AController* C = Pawn->GetController())
        {
            C->GetPlayerViewPoint(Start, Dir.Rotation());
            Dir = Dir.Rotation().Vector();
        }
        else
        {
            Start = Pawn->GetActorLocation();
            Dir = Pawn->GetActorForwardVector();
        }
    }
    else
    {
        Start = Owner->GetActorLocation();
        Dir = Owner->GetActorForwardVector();
    }

    FVector End = Start + Dir * MaxDistance;
    FHitResult Hit;
    GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel);

    AActor* NewFocus = Hit.GetActor();
    if (NewFocus != FocusedActor.Get())
    {
        AActor* Old = FocusedActor.Get();
        FocusedActor = NewFocus;
        OnFocusChanged.Broadcast(NewFocus, Old);
    }
}

void UInteractionComponent::Interact()
{
    if (!FocusedActor.IsValid()) return;
    ServerInteract(FocusedActor.Get());
}

void UInteractionComponent::ServerInteract_Implementation(AActor* Target)
{
    if (!Target) return;
    if (UInteractableComponent* Comp = Target->FindComponentByClass<UInteractableComponent>())
    {
        if (!Comp->IsAvailable(GetOwner())) return;
        Comp->Begin(GetOwner());
        if (Comp->HoldTime <= 0.f)
        {
            Comp->Complete(GetOwner());
        }
        else
        {
            // For simplicity, immediate complete; extend with timers if needed.
            Comp->Complete(GetOwner());
        }
    }
    // Specialized interfaces
    if (Target->GetClass()->ImplementsInterface(UTradeInteractable::StaticClass()))
    {
        ITradeInteractable::Execute_RequestTrade(Target, GetOwner());
    }
    if (Target->GetClass()->ImplementsInterface(UShopInteractable::StaticClass()))
    {
        IShopInteractable::Execute_OpenShop(Target, GetOwner());
    }
}

bool UInteractionComponent::ServerInteract_Validate(AActor* Target)
{
    return true;
}
