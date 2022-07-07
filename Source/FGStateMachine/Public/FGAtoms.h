#pragma once

#include "CoreMinimal.h"
#include "SM_State.h"
#include "FGAtoms.generated.h"

UENUM(Blueprintable, meta=(Bitflags))
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

UENUM()
enum class EFGButtonState : uint8
{
	Up,
	JustPressed,
	HeldDown,
	Count UMETA(Hidden)
	
};

UENUM()
enum class EFGInputSticks : uint8
{
	LeftStick,
	Count UMETA(Hidden)
	
};

UENUM(Blueprintable, meta=(Bitflags))
enum class EFGInputButtons : uint8
{
	LeftFace,
	TopFace,
	Count UMETA(Hidden)
	
};

/**
 * 
 */
UCLASS()
class FGSTATEMACHINE_API UFGDirectionalInputAtom : public USM_InputAtom
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
UCLASS()
class FGSTATEMACHINE_API UFGButtonInputAtom : public USM_InputAtom
{
	GENERATED_BODY()
	
public:
	// State of the button being described.
	UPROPERTY(EditAnywhere)
	EFGButtonState ButtonState;
	
};
