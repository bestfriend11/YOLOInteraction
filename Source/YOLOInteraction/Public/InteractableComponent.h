#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionInterfaces.h"
#include "InteractableComponent.generated.h"

UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class YOLOINTERACTION_API UInteractableComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UInteractableComponent();

    /** Prompt shown to the player when focused. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    FText Prompt;

    /** Hold time required to complete interaction (0 = instant). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    float HoldTime = 0.f;

    /** Max simultaneous users (0/1 = exclusive). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    int32 MaxUsers = 1;

    /** Returns true if actor implements IInteractable and is available. */
    bool IsAvailable(AActor* Interactor) const;

    void Begin(AActor* Interactor);
    void Complete(AActor* Interactor);
    void Cancel(AActor* Interactor);

private:
    int32 ActiveUsers = 0;
};
