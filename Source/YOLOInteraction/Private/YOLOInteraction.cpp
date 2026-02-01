#include "YOLOInteraction.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "Engine/Engine.h"

int32 GYOLOInteractionDebug = 0;
static FAutoConsoleVariableRef CVarYOLOInteractionDebug(
	TEXT("yolo.interaction.debug"),
	GYOLOInteractionDebug,
	TEXT("Enable on-screen debug for YOLOInteraction (0=off,1=basic,2=full overlay)."),
	ECVF_Default);

static FAutoConsoleCommand GToggleInteractionDebugCmd(
	TEXT("yolo_interaction_debug"),
	TEXT("Set YOLOInteraction debug level (0 off, 1 basic, 2 full overlay)."),
	FConsoleCommandWithArgsDelegate::CreateStatic([](const TArray<FString>& Args)
	{
		if (Args.Num() > 0)
		{
			GYOLOInteractionDebug = FCString::Atoi(*Args[0]);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
					FString::Printf(TEXT("Interaction debug %s (level %d)"),
						GYOLOInteractionDebug > 0 ? TEXT("ON") : TEXT("OFF"), GYOLOInteractionDebug));
			}
		}
	}));

IMPLEMENT_MODULE(FYOLOInteractionModule, YOLOInteraction)

void FYOLOInteractionModule::StartupModule() {}
void FYOLOInteractionModule::ShutdownModule() {}
