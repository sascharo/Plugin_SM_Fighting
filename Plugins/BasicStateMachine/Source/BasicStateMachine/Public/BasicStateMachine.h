#pragma once

#include "CoreMinimal.h"
//#include "Styling/SlateStyle.h"
//#include "Styling/SlateStyleRegistry.h"

class FBasicStateMachineModule : public IModuleInterface
{
public:
	// IModuleInterface implementation.
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
