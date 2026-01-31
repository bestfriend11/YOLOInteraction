# YOLOInteraction

Lightweight, reusable interaction framework for Unreal Engine, built to pair with YOLOInventory but usable standalone.

## Features
- `InteractionComponent`: client-side trace + focus events, server-validated Interact RPC.
- `InteractableComponent`: drop on any actor; holds prompt/hold-time/max-users, calls interfaces.
- Interfaces: `IInteractable`, `IShopInteractable`, `ITradeInteractable` (extend as needed).
- UMG/Slate friendly: expose prompt text and focus change events; keep UI layer separate.
- Networking: client finds target, server revalidates range/LOS and executes interact callbacks.

## Quick setup
1) Enable plugin in your project.
2) Add `InteractionComponent` to your Pawn/Controller; bind your Interact input to `Interact()`.
3) Add `InteractableComponent` to actors you want to use; implement `IInteractable` (and/or `IShopInteractable`, `ITradeInteractable`).
4) For shop/trade: in `CompleteInteract`, call into your shop/trade component (e.g., `UYIShopComponent::ServerBuyItem` or `YITradeInteractionComponent::RequestTrade`).

## Extending
- Add more interfaces (loot, dialogue, door) without changing the core components.
- Customize tracing (distance/channel) and prompts via component properties.
- If you need hold-to-interact progress UI, bind a progress widget to the hold time and drive it from input duration.

## Repo layout
- `Source/YOLOInteraction/Public` – Interfaces and components headers.
- `Source/YOLOInteraction/Private` – Component logic and module startup.
- `YOLOInteraction.uplugin` – plugin descriptor.

## Notes
- Components are lightweight: no ticking on interactables; only the interaction component ticks traces.
- Security: server validates the target on Interact RPC; add LOS/radius checks as needed for your game.
- Compatible with YOLOInventory shops/trades but does not depend on them.
