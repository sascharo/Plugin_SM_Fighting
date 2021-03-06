#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FGDefaultPawn.h"
#include "FGState.h"
#include "SM_State.h"
#include "FGMoveLink.generated.h"

class UFGMove;

/**
 * 
 */
UCLASS()
class PLUGIN_SM_FIGHTING_API UFGMoveLink : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FStateMachineResult TryLink(const AFGDefaultPawn* DefaultPawn,
							    const TArray<USM_InputAtom*>& InputAtoms,
							    int32 DataIndex=0,
								int32 RemainingSteps=-1);

	UPROPERTY(EditAnywhere, Category="Move Link")
	UFGMove* Move;

	// Set of windows (in seconds) when this link can be used.
	UPROPERTY(EditAnywhere, Category="Move Link")
	TArray<FVector2D> CancelWindows;

	// Clears input buffer when using this link.
	UPROPERTY(EditAnywhere, Category="Move Link")
	uint32 bClearInput : 1;

	// Sequence of inputs used to trigger this move. If not NULL, will override move's own input state machine.
	UPROPERTY(EditAnywhere, Category="Move Link")
	UFGState* InputStateMachine;
	
};
