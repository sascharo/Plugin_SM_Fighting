#include "FGInputTransition.h"

USM_State* UFGInputTransition::TryTransition(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, const int32 DataIndex, int32& OutDataIndex)
{
	OutDataIndex = DataIndex;
	
	return nullptr;
}
