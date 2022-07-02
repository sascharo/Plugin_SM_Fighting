#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SM_State.generated.h"

class USM_InputAtom;
class USM_Transition;
class USM_State;

UENUM()
enum class EStateMachineCompletionType: uint8
{
	// Implicit failure - this state is not marked as accept state.
	NotAccepted,
	// Success - this state is an accept state.
	Accepted,
	// Explicit failure  - this state is specifically marked as failure/not-accept state.
	Rejected,
	// Simulation run out of steps while machine was still running.
	OutOfSteps UMETA(Hidden)
	
};

USTRUCT()
struct BASICSTATEMACHINE_API FStateMachineResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	EStateMachineCompletionType CompletionType;

	UPROPERTY()
	USM_State* FinalState;

	UPROPERTY()
	int32 DataIndex;

	FStateMachineResult(): CompletionType(), FinalState(nullptr), DataIndex(0) {}

	FStateMachineResult(const EStateMachineCompletionType StateMachineCompletionType, USM_State* USMFinalState, int32 DataIdx)
	{
		CompletionType = StateMachineCompletionType;
		FinalState = USMFinalState;
		DataIndex = DataIdx;
	}
	
};

UCLASS()
class BASICSTATEMACHINE_API USM_InputAtom : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Display value for this input atom, mainly for debugging purposes.
	UPROPERTY(EditAnywhere)
	FName Description;
	
};

UCLASS(EditInlineNew)
class BASICSTATEMACHINE_API USM_Transition : public UDataAsset
{
	GENERATED_BODY()

public:
	// Returns DestinationState on success, NULL on failure.
	// For subclasses, OutDataIndex might be something other than 1, if a transition is made to consume multiple inputs.
	virtual USM_State* TryTransition(const UObject* RefObject,
									 const TArray<USM_InputAtom*>& DataSource,
									 const int32 DataIndex,
									 int32 &OutDataIndex);

protected:
	// State where we will go next if this transition is taken. If null, this transition will be ignored.
	UPROPERTY(EditAnywhere)
	USM_State* DestinationState;

	// If true, the meaning of AcceptableInputs is reserved.
	UPROPERTY(EditAnywhere)
	uint32 bReverseInputTest : 1;

	// Acceptable inputs, the current input atom must be on this list.
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> AcceptableInputs;
	
};

UCLASS()
class BASICSTATEMACHINE_API USM_State : public UDataAsset
{
	GENERATED_BODY()

public:
	USM_State()
	{
		bLoopByDefault = true;
	}

	// Attempt to run the state with input from the given DataSource object.
	// Will start reading input at DataIndex.
	// Will decrement RemainingSteps and automatically fail after it hits 0.
	UFUNCTION(Blueprintable, Category="State Machine")
	virtual FStateMachineResult RunState(const UObject* RefObject,
										 const TArray<USM_InputAtom*>& DataSource,
										 const int32 DataIndex=0,
										 int32 RemainingSteps=-1);

protected:
	// Loop, used when input atom being processed isn't recognized.
	virtual FStateMachineResult LoopState(const UObject* RefObject,
										  const TArray<USM_InputAtom*>& DataSource,
										  int32 DataIndex,
										  int32 RemainingSteps);

	// If input runs out on this state (or TerminateImmediately is true), this is how the result will be interpreted.
	UPROPERTY(EditAnywhere)
	EStateMachineCompletionType CompletionType;

	// A state machine run that enters this state will terminate immediately, regardless of whether or not there is more input data.
	UPROPERTY(EditAnywhere)
	uint32 bTerminateImmediately : 1;

	// If this is set, this state will loop on itself whenever an unhandled input atom is detected.
	UPROPERTY(EditAnywhere)
	uint32 bLoopByDefault : 1;
	
	// Transitions to other states. These are in priority order, so the first successful branch will be taken.
	UPROPERTY(EditAnywhere, Instanced)
	TArray<USM_Transition*> InstancedTransitions;

	// Transitions to other states.
	// These are in priority order, so the first successful branch will be taken. These run after InstancedTransitions.
	UPROPERTY(EditAnywhere)
	TArray<USM_Transition*> SharedTransitions;
	
};
