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
}

void AInteractionPad::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    OnPadEntered.Broadcast(Other);
    if (GYOLOInteractionDebug && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.5f, FColor::Green,
            FString::Printf(TEXT("[Pad] %s entered %s"), *GetNameSafe(Other), *GetNameSafe(this)));
    }
}

void AInteractionPad::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
    OnPadExited.Broadcast(Other);
    if (GYOLOInteractionDebug && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.5f, FColor::Red,
            FString::Printf(TEXT("[Pad] %s exited %s"), *GetNameSafe(Other), *GetNameSafe(this)));
    }
}
