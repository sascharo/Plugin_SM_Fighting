#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SM_State.h"
#include "FGMoveLink.h"
#include "FGMove.generated.h"

class AFGDefaultPawn;
//class UFGMoveLink;

/**
 * 
 */
USTRUCT(Category="Fighting Game")
struct PLUGIN_SM_FIGHTING_API FFGMoveLinkToFollow
{
	GENERATED_USTRUCT_BODY()
public:
	FORCEINLINE FFGMoveLinkToFollow() {	Link = nullptr;	}

	UPROPERTY()
	UFGMoveLink* Link;

	UPROPERTY()
	FStateMachineResult SMResult;
	
};

/**
 * 
 */
UCLASS(Category="Fighting Game")
class PLUGIN_SM_FIGHTING_API UFGMove : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Tries all links, returns the best one to follow and the result of the state machine that got there.
	FFGMoveLinkToFollow TryLinks(const AFGDefaultPawn* DefaultPawn,
								 const TArray<USM_InputAtom*>& InputAtoms,
								 int32 DataIndex=0,
								 int32 RemainingSteps=-1);
	
	// Animation to play while doing this move.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	// Animation to play while doing this move.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Animation;
	
	// Links from this move to others.
	UPROPERTY(EditAnywhere)
	TArray<UFGMoveLink*> LinkedMoves;

	// Clears input buffer when entering this state.
	UPROPERTY(EditAnywhere)
	uint32 bClearInputOnEntry : 1;

	// Clears input buffer when exiting this state.
	UPROPERTY(EditAnywhere)
	uint32 bClearInputOnExit : 1;
	
};
