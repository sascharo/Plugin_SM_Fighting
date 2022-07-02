#include "BasicStateMachine.h"

#define LOCTEXT_NAMESPACE "FBasicStateMachineModule"

void FBasicStateMachineModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module.
	UE_LOG(LogTemp, Warning, TEXT("Basic State Machine Module loaded!"));
}

void FBasicStateMachineModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBasicStateMachineModule, BasicStateMachine)
