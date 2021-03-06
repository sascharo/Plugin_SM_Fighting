#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "SM_State.h"
#include "FGDefaultPawn.generated.h"

class UFGDirectionalInputAtom;
class UFGButtonInputAtom;
class UFGMove;

/**
 * 
 */
UCLASS()
class PLUGIN_SM_FIGHTING_API AFGDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()
	
public:	
	AFGDefaultPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	FORCEINLINE float GetTimeInMove() const { return TimeInCurrentMove; }

protected:
	void LeftButtonPressed();
	void LeftButtonReleased();
	void TopButtonPressed();
	void TopButtonReleased();
	
	void ReadXAxis(float Value);
	void ReadYAxis(float Value);
	
	void UseGameCamera();

	UFUNCTION(BlueprintImplementableEvent)
	void DoMove(UFGMove* NewMove);

public:
	// Input atoms are removed when they pass this age threshold. All moves must be executed in under this time.
	// This needs to be tweaked for your game's feel, but it works.
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.f, ClampMax=10.f))
	float InputExpirationTime = 0.75f; // 1.f;
	
	UPROPERTY(EditAnywhere, meta=(DisplayName="Directional Threshold", ClampMin=0.f, ClampMax=10.f, Category="Tick"))
	float DirectionThreshold = 0.5f;

protected:
	FVector2D DirectionInput;
	int32 ButtonsDown = 0, ButtonsDown_Old = 0;
	float TimeInCurrentMove = 0.f;

	UPROPERTY(EditAnywhere, Category="Move")
	UFGMove* CurrentMove = nullptr;

	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionDownBackAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionDownAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionDownForwardAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionBackAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionNeutralAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionForwardAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionUpBackAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionUpAtom;
	UPROPERTY(EditAnywhere, Category="Directional Input Atoms")
	UFGDirectionalInputAtom* DirectionUpForwardAtom;

	// Order in this array is the same as EFGButtonState: Up, JustPressed, Held
	UPROPERTY(EditAnywhere, Category="Button Input Atoms")
	TArray<UFGButtonInputAtom*> ButtonAtoms;

private:
	//~ This array relates to InputStream. InputStream must not be updated without this stream being updated as well.
	UPROPERTY(VisibleInstanceOnly, Category="Input")
	TArray<float> InputTimeStamps;
	
	UPROPERTY(VisibleInstanceOnly, Category="Input")
	TArray<USM_InputAtom*> InputStream;
	
};
