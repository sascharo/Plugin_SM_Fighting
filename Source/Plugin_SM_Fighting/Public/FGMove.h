#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SM_State.h"
#include "FGMove.generated.h"

class AFGDefaultPawn;
class UFGMoveLink;

/**
 * 
 */
USTRUCT(Category="Fighting Game")
struct PLUGIN_SM_FIGHTING_API FFGMoveLinkToFollow
{
	GENERATED_USTRUCT_BODY()
public:
	FFGMoveLinkToFollow() {	Link = nullptr;	}

	UPROPERTY()
	UFGMoveLink* Link;

	UPROPERTY()
	FStateMachineResult SMR;
	
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
	FFGMoveLinkToFollow TryLinks(const AFGDefaultPawn* RefObject,
								 const TArray<USM_InputAtom*>& DataSource,
								 int32 DataIndex=0,
								 int32 RemainingSteps=-1);

	// Animation to play while doing this move.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Move")
	FName MoveName;

	// Animation to play while doing this move.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Move")
	UAnimMontage* Animation;
	
	// Links from this move to others.
	UPROPERTY(EditAnywhere, Category="Move")
	TArray<UFGMoveLink*> LinkedMoves;

	// Clears input buffer when entering this state.
	UPROPERTY(EditAnywhere, Category="Move")
	uint32 bClearInputOnEntry : 1;

	// Clears input buffer when exiting this state.
	UPROPERTY(EditAnywhere, Category="Move")
	uint32 bClearInputOnExit : 1;
	
};
