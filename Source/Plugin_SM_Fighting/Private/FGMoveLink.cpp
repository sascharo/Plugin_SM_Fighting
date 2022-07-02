#include "FGMoveLink.h"

FStateMachineResult UFGMoveLink::TryLink(const AFGDefaultPawn* RefObject,
											const TArray<USM_InputAtom*>& DataSource,
											int32 DataIndex/*=0*/,
											int32 RemainingSteps/*=-1*/)
{
	if (InputStateMachine && Move)
	{
		bool bCanCancel = false;
		
		for (const FVector2D& CancelWindow : CancelWindows)
		{
			if (RefObject->GetTimeInMove() == FMath::Clamp(RefObject->GetTimeInMove(), CancelWindow.X, CancelWindow.Y))
			{
				bCanCancel = true;
				
				break;
			}
		}
		
		// If no cancel windows are provided, assume the move is always available.
		if (bCanCancel || !CancelWindows.Num())
		{
			return InputStateMachine->RunState(RefObject, DataSource, DataIndex, RemainingSteps);
		}
	}
	
	return FStateMachineResult();
}
