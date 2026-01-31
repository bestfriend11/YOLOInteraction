#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionPad.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPadActorChanged, AActor*, Actor);

/** Simple overlap-based pad that fires enter/exit events; useful for pressure plates/triggers. */
UCLASS()
class YOLOINTERACTION_API AInteractionPad : public AActor
{
    GENERATED_BODY()
public:
    AInteractionPad();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* Trigger;

    UPROPERTY(BlueprintAssignable, Category="Pad")
    FOnPadActorChanged OnPadEntered;

    UPROPERTY(BlueprintAssignable, Category="Pad")
    FOnPadActorChanged OnPadExited;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit);
    UFUNCTION()
    void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex);
};
