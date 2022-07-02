#pragma once

#include "CoreMinimal.h"
#include "SM_State.h"
#include "FGAtoms.generated.h"

UENUM(Blueprintable, meta=(Bitflags), Category="State Machine")
enum class EFGInputDirections : uint8
{
	DownBack,
	Down,
	DownForward,
	Back,
	Neutral,
	Forward,
	UpBack,
	Up,
	UpForward
	
};

UENUM(Category="State Machine")
enum class EFGButtonState : uint8
{
	Up,
	JustPressed,
	HeldDon,
	Count UMETA(Hidden)
	
};

UENUM(Blueprintable, meta=(Bitflags), Category="State Machine")
enum class EFGInputButtons : uint8
{
	LeftFace,
	TopFace,
	Count UMETA(Hidden)
	
};

/**
 * 
 */
UCLASS(Category="State Machine")
class PLUGIN_SM_FIGHTING_API UFGDirectionalInputAtom : public USM_InputAtom
{
	GENERATED_BODY()
public:
	// Which direction we're describing with this atom.
	UPROPERTY(EditAnywhere)
	EFGInputDirections InputDirection;
	
};

/**
 * 
 */
UCLASS(Category="State Machine")
class PLUGIN_SM_FIGHTING_API UFGButtonInputAtom : public USM_InputAtom
{
	GENERATED_BODY()
public:
	// State of the button being described.
	UPROPERTY(EditAnywhere)
	EFGInputDirections InputDirection;
	
};
