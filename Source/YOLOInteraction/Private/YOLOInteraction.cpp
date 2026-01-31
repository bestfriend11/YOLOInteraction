#include "YOLOInteraction.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"

int32 GYOLOInteractionDebug = 0;
static FAutoConsoleVariableRef CVarYOLOInteractionDebug(
	TEXT("yolo.interaction.debug"),
	GYOLOInteractionDebug,
	TEXT("Enable on-screen debug for YOLOInteraction (0=off,1=basic)."),
	ECVF_Default);

IMPLEMENT_MODULE(FYOLOInteractionModule, YOLOInteraction)

void FYOLOInteractionModule::StartupModule() {}
void FYOLOInteractionModule::ShutdownModule() {}
