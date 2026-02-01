#include "InteractionComponent.h"
#include "InteractableComponent.h"
#include "InteractionInterfaces.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "YOLOInteraction.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/OverlapResult.h"

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
    APawn* ViewPawn = GetViewPawn();
    FVector Start;
    FVector Dir;
    if (ViewPawn)
    {
        Start = ViewPawn->GetActorLocation();
        Dir = ViewPawn->GetActorForwardVector();
    }
    else
    {
        Start = Owner->GetActorLocation();
        Dir = Owner->GetActorForwardVector();
    }

    FVector End = Start + Dir * MaxDistance;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionTrace), false, Owner);
    Params.AddIgnoredActor(Owner);
    if (ViewPawn) Params.AddIgnoredActor(ViewPawn);

    if (TraceRadius > KINDA_SMALL_NUMBER)
    {
        GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(TraceRadius), Params);
    }
    else
    {
        GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, Params);
    }

    // Collect nearby interactables for debug overlays
    DebugCandidates.Reset();
    if (GYOLOInteractionDebug > 1 && DebugScanRadius > 0.f)
    {
        TArray<FOverlapResult> Overlaps;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(DebugScanRadius);
        GetWorld()->OverlapMultiByChannel(Overlaps, Start, FQuat::Identity, TraceChannel, Sphere, Params);
        for (const FOverlapResult& O : Overlaps)
        {
            if (AActor* A = O.GetActor())
            {
                if (A->FindComponentByClass<UInteractableComponent>())
                {
                    DebugCandidates.Add(A);
                }
            }
        }
    }

    AActor* NewFocus = Hit.GetActor();
    if (NewFocus != FocusedActor.Get())
    {
        AActor* Old = FocusedActor.Get();
        FocusedActor = NewFocus;
        OnFocusChanged.Broadcast(NewFocus, Old);
        if (GYOLOInteractionDebug && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Yellow,
                FString::Printf(TEXT("[Interaction] Focus -> %s"), *GetNameSafe(NewFocus)));
        }
    }

    if (GYOLOInteractionDebug)
    {
        DrawDebug(Start, End, Hit, DebugCandidates);
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
    AActor* InstigatorActor = Cast<AActor>(GetViewPawn());
    if (!InstigatorActor) InstigatorActor = GetOwner();

    if (UInteractableComponent* Comp = Target->FindComponentByClass<UInteractableComponent>())
    {
        if (!Comp->IsAvailable(InstigatorActor)) return;
        Comp->Begin(InstigatorActor);
        if (Comp->HoldTime <= 0.f)
        {
            Comp->Complete(InstigatorActor);
        }
        else
        {
            // For simplicity, immediate complete; extend with timers if needed.
            Comp->Complete(InstigatorActor);
        }
        if (GYOLOInteractionDebug && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green,
                FString::Printf(TEXT("[Interaction] Used %s"), *GetNameSafe(Target)));
        }
    }
    // Specialized interfaces
    if (Target->GetClass()->ImplementsInterface(UTradeInteractable::StaticClass()))
    {
        ITradeInteractable::Execute_RequestTrade(Target, InstigatorActor);
    }
    if (Target->GetClass()->ImplementsInterface(UShopInteractable::StaticClass()))
    {
        IShopInteractable::Execute_OpenShop(Target, InstigatorActor);
    }
}

bool UInteractionComponent::ServerInteract_Validate(AActor* Target)
{
    return true;
}

void UInteractionComponent::yolo_interaction_debug(int32 bEnable)
{
    GYOLOInteractionDebug = bEnable;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
            FString::Printf(TEXT("Interaction debug %s"), GYOLOInteractionDebug ? TEXT("ON") : TEXT("OFF")));
    }
}

void UInteractionComponent::DrawDebug(const FVector& Start, const FVector& End, const FHitResult& Hit, const TArray<TWeakObjectPtr<AActor>>& Candidates)
{
    const bool bHit = Hit.GetActor() != nullptr;
    const FColor LineColor = bHit ? FColor::Green : FColor::Red;
    DrawDebugLine(GetWorld(), Start, End, LineColor, false, DebugDrawTime, 0, 1.5f);
    if (bHit)
    {
        DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius > 0.f ? TraceRadius : 8.f, 12, FColor::Green, false, DebugDrawTime);
        DrawDebugString(GetWorld(), Hit.ImpactPoint + FVector(0,0,20.f), GetNameSafe(Hit.GetActor()), nullptr, FColor::Green, DebugDrawTime, true);
    }
    if (GYOLOInteractionDebug > 1)
    {
        for (const TWeakObjectPtr<AActor>& WeakA : Candidates)
        {
            if (AActor* A = WeakA.Get())
            {
                const FVector Loc = A->GetActorLocation();
                const float Dist = FVector::Distance(Start, Loc);
                const bool bIsFocus = (A == Hit.GetActor());
                DrawDebugSphere(GetWorld(), Loc, 18.f, 8, bIsFocus ? FColor::Emerald : FColor::Cyan, false, DebugDrawTime);
                DrawDebugString(GetWorld(), Loc + FVector(0,0,30.f),
                    FString::Printf(TEXT("%s (%.0f)"), *A->GetName(), Dist),
                    nullptr, bIsFocus ? FColor::Emerald : FColor::Cyan, DebugDrawTime, true);
            }
        }
        DrawDebugString(GetWorld(), Start + FVector(0,0,60.f),
            FString::Printf(TEXT("Focus:%s Dist:%.0f"), *GetNameSafe(Hit.GetActor()), Hit.Distance),
            nullptr, FColor::White, DebugDrawTime, true);
    }
}

APawn* UInteractionComponent::GetViewPawn() const
{
    if (APawn* PawnOwner = Cast<APawn>(GetOwner()))
    {
        return PawnOwner;
    }
    if (const AController* PC = Cast<AController>(GetOwner()))
    {
        return PC->GetPawn();
    }
    return nullptr;
}
