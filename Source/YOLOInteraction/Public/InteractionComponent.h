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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

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

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerInteract(AActor* Target);
};
