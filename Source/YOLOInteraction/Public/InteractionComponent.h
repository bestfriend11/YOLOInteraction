#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UInteractableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionFocusChanged, AActor*, NewActor, AActor*, OldActor);

UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class YOLOINTERACTION_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UInteractionComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    float MaxDistance = 400.f;

    /** Optional sphere radius for focus trace (0 = line trace). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    float TraceRadius = 12.f;

    /** Debug: extra overlap radius to list nearby interactables (only used when debug is on). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Debug")
    float DebugScanRadius = 200.f;

    /** Debug: duration for debug shapes per tick. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Debug")
    float DebugDrawTime = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel1; // default: Interaction channel

    UPROPERTY(BlueprintAssignable, Category="Interaction|Events")
    FOnInteractionFocusChanged OnFocusChanged;

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void Interact();

    /** Exec helper: toggle debug overlay for interaction (client only). */
    UFUNCTION(Exec)
    void yolo_interaction_debug(int32 bEnable);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    TWeakObjectPtr<AActor> FocusedActor;
    void UpdateFocus();
    void DrawDebug(const FVector& Start, const FVector& End, const FHitResult& Hit, const TArray<TWeakObjectPtr<AActor>>& Candidates);
    APawn* GetViewPawn() const;

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerInteract(AActor* Target);

    TArray<TWeakObjectPtr<AActor>> DebugCandidates;
};
