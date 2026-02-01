#include "InteractionPad.h"
#include "Components/BoxComponent.h"
#include "YOLOInteraction.h"

AInteractionPad::AInteractionPad()
{
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    RootComponent = Trigger;
    Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Trigger->SetGenerateOverlapEvents(true);
}

void AInteractionPad::BeginPlay()
{
    Super::BeginPlay();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AInteractionPad::HandleBeginOverlap);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &AInteractionPad::HandleEndOverlap);
    OverlappingActors.Reset();
}

void AInteractionPad::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (!Other || Other == this) return;
    if (bServerOnly && !HasAuthority()) return;
    if (bOnlyPawns && !Other->IsA<APawn>()) return;

    const TWeakObjectPtr<AActor> WeakOther(Other);
    if (OverlappingActors.Contains(WeakOther)) return; // already inside

    OverlappingActors.Add(WeakOther);
    OnPadEntered.Broadcast(Other);
    if (GYOLOInteractionDebug && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.5f, FColor::Green,
            FString::Printf(TEXT("[Pad] %s entered %s"), *GetNameSafe(Other), *GetNameSafe(this)));
    }
}

void AInteractionPad::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
    if (!Other || Other == this) return;
    if (bServerOnly && !HasAuthority()) return;

    const TWeakObjectPtr<AActor> WeakOther(Other);
    if (OverlappingActors.Remove(WeakOther) > 0)
    {
        OnPadExited.Broadcast(Other);
        if (GYOLOInteractionDebug && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.5f, FColor::Red,
                FString::Printf(TEXT("[Pad] %s exited %s"), *GetNameSafe(Other), *GetNameSafe(this)));
        }
    }
    else if (GYOLOInteractionDebug > 1 && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.5f, FColor::Yellow,
            FString::Printf(TEXT("[Pad] %s exit ignored (not tracked)"), *GetNameSafe(Other)));
    }
}
