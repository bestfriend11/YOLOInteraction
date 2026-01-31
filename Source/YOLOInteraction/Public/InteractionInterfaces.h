#pragma once

#include "UObject/Interface.h"
#include "InteractionInterfaces.generated.h"

UINTERFACE(Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class IInteractable
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    bool CanInteract(AActor* Interactor) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void BeginInteract(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void CompleteInteract(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void CancelInteract(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    FText GetInteractionPrompt() const;
};

UINTERFACE(Blueprintable)
class UShopInteractable : public UInterface
{
    GENERATED_BODY()
};

class IShopInteractable
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void OpenShop(AActor* Interactor);
};

UINTERFACE(Blueprintable)
class UTradeInteractable : public UInterface
{
    GENERATED_BODY()
};

class ITradeInteractable
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void RequestTrade(AActor* Interactor);
};
