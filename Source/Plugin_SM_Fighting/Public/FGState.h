#pragma once

#include "CoreMinimal.h"
#include "SM_State.h"
#include "FGState.generated.h"

/**
 * 
 */
UCLASS(Category="Fighting Game")
class PLUGIN_SM_FIGHTING_API UFGState : public USM_State
{
	GENERATED_BODY()

public:
	virtual FStateMachineResult LoopState(const UObject* RefObject,
										  const TArray<USM_InputAtom*>& DataSource,
										  int32 DataIndex,
										  int32 RemainingSteps) override;
	
};
