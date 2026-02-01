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

    /** If true and owner implements IInteractable, defer to its CanInteract/Begin/Complete/Cancel implementations. Disable to rely only on this component. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    bool bConsultOwnerImplementation = true;

    /** Automatically make the owner's root component block the Interaction channel (fallback to Visibility) at BeginPlay. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Collision")
    bool bAutoEnableInteractionCollision = true;

    /** Returns true if actor implements IInteractable and is available. */
    bool IsAvailable(AActor* Interactor) const;

    void Begin(AActor* Interactor);
    void Complete(AActor* Interactor);
    void Cancel(AActor* Interactor);

private:
    int32 ActiveUsers = 0;
    mutable bool bInOwnerCall = false;

protected:
    virtual void BeginPlay() override;
};
