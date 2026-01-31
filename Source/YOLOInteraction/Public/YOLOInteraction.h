#pragma once
#include "Modules/ModuleManager.h"

class FYOLOInteractionModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
