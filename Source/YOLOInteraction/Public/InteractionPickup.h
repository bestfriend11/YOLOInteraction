#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YIInventoryTypes.h" // for FYIBagItem deps
#include "InteractionInterfaces.h"
#include "InteractionPickup.generated.h"

class UStaticMeshComponent;
class UInteractableComponent;
class UYIItemDefinition;
class UYIInventoryComponent;

/** Simple world pickup that gives an item to the interactor's inventory. */
UCLASS()
class YOLOINTERACTION_API AInteractionPickup : public AActor, public IInteractable
{
    GENERATED_BODY()
public:
    AInteractionPickup();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInteractableComponent* Interactable;

    /** Item to grant. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
    TSoftObjectPtr<UYIItemDefinition> ItemDef;

    /** Stack count. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
    int32 Count = 1;

    /** Destroy actor after successful pickup. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
    bool bDestroyOnPickup = true;

    virtual bool CanInteract_Implementation(AActor* Interactor) const override;
    virtual void BeginInteract_Implementation(AActor* Interactor) override;
    virtual void CompleteInteract_Implementation(AActor* Interactor) override;
    virtual FText GetInteractionPrompt_Implementation() const override;

private:
    bool GiveTo(AActor* Interactor);
};
