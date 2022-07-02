#pragma once

#include "CoreMinimal.h"
#include "SM_State.h"
#include "FGInputTransition.generated.h"

/**
 * 
 */
UCLASS(Category="Fighting Game")
class PLUGIN_SM_FIGHTING_API UFGInputTransition : public USM_TransitionBase
{
	GENERATED_BODY()

public:
	// Consume 2 types of input - Direction + Button(s).
	virtual USM_State* TryTransition(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, const int32 DataIndex, int32& OutDataIndex) override;

protected:
	// These buttons must be Just Pressed or Held Down.
	// If any are checked, at least one must be Just Pressed.
	UPROPERTY(EditAnywhere, meta=(Bitmask, BitmaskEnum="EFGInputButtons"))
	int32 RequiredButtons;

	// These buttons must all be Up.
	UPROPERTY(EditAnywhere, meta=(Bitmask, BitmaskEnum="EFGInputButtons"))
	int32 ForbiddenButtons;

	// Required control-stick directions - at least one of these must match the current input direction.
	UPROPERTY(EditAnywhere, meta=(Bitmask, BitmaskEnum="EFGInputDirections"))
	int32 AcceptableDirections;
	
};
