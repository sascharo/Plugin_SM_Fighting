#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SM_State.h"
#include "FGMoveLink.generated.h"

class AFGDefaultPawn;
class UFGState;
class UFGMove;

/**
 * 
 */
UCLASS()
class PLUGIN_SM_FIGHTING_API UFGMoveLink : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FStateMachineResult TryLink(const AFGDefaultPawn* RefObject,
								const TArray<USM_InputAtom*>& DataSource,
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
