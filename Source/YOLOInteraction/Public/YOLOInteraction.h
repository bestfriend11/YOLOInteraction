#pragma once
#include "Modules/ModuleManager.h"

class FYOLOInteractionModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

// Global debug flag (set via console cvar yolo.interaction.debug)
extern int32 GYOLOInteractionDebug;
