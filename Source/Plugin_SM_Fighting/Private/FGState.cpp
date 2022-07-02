#include "FGState.h"
#include "FGAtoms.h"

FStateMachineResult UFGState::LoopState(const UObject* RefObject,
                                        const TArray<USM_InputAtom*>& DataSource,
                                        int32 DataIndex,
                                        int32 RemainingSteps)
{
	return RunState(RefObject, DataSource, DataIndex + 1 + static_cast<int32>(EFGInputButtons::Count), RemainingSteps - 1);
}
