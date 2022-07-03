#include "FGDefaultPawn.h"
#include "FGControlsGameMode.h"
#include "FGCameraActor.h"
#include "FGAtoms.h"
#include "FGMove.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AFGDefaultPawn::AFGDefaultPawn()
{
	// Needed because we're using DefaultPawn.
	bAddDefaultMovementBindings = false;
}

void AFGDefaultPawn::BeginPlay() 
{
	Super::BeginPlay();

	if (UStaticMeshComponent* SMC = GetMeshComponent())
	{
		SMC->SetHiddenInGame(true);
	}

	if (!CurrentMove)
	{
		UE_LOG(LogTemp, Warning, TEXT("No initial move."));
		Destroy();
		
		return;
	}
	// Check that we have all the states we need.
	if (!DirectionDownBackAtom || !DirectionDownAtom || !DirectionDownForwardAtom ||
		!DirectionBackAtom || !DirectionNeutralAtom || !DirectionForwardAtom ||
		!DirectionUpBackAtom || !DirectionUpAtom || !DirectionUpForwardAtom)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing at least one directional atom."));
		Destroy();
		
		return;
	}

	constexpr auto ButtonInputCount = static_cast<int32>(EFGButtonState::Count);

	for (int32 i = 0; i < ButtonInputCount; ++i)
	{
		if (!ButtonAtoms.IsValidIndex(i) || !ButtonAtoms[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough button input atoms, or a NULL entry was found in the list."));
			Destroy();
			
			return;
		}
	}

	GetWorldTimerManager().SetTimerForNextTick(this, &AFGDefaultPawn::UseGameCamera);
}

void AFGDefaultPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Process input.

	//const auto NegDirectionThreshold = -DirectionThreshold;
	
	// Add one atom for stick direction.
	UFGDirectionalInputAtom* InputDirection = nullptr;
	
	if (DirectionInput.X < -DirectionThreshold)
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			InputDirection = DirectionDownBackAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			InputDirection = DirectionBackAtom;
		}
		else
		{
			InputDirection = DirectionUpBackAtom;
		}
	}
	else if (DirectionInput.X < DirectionThreshold)
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			InputDirection = DirectionDownAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			InputDirection = DirectionNeutralAtom;
		}
		else
		{
			InputDirection = DirectionUpAtom;
		}
	}
	else
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			InputDirection = DirectionDownForwardAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			InputDirection = DirectionForwardAtom;
		}
		else
		{
			InputDirection = DirectionUpForwardAtom;
		}
	}
	
	InputStream.Add(InputDirection);

	// Add one atom for each button's state.
	for (int32 i = 0; i < static_cast<int32>(EFGInputButtons::Count); ++i)
	{
		if (ButtonsDown & (1 << i))
		{
			if (ButtonsDown_Old & (1 << i))
			{
				InputStream.Add(ButtonAtoms[static_cast<int32>(EFGButtonState::HeldDown)]);
			}
			else
			{
				InputStream.Add(ButtonAtoms[static_cast<int32>(EFGButtonState::JustPressed)]);
			}
		}
		else
		{
			InputStream.Add(ButtonAtoms[static_cast<int32>(EFGButtonState::Up)]);
		}
	}

	// Cache old button state so we can distinguish between held and just pressed.
	ButtonsDown_Old = ButtonsDown;
	
	// Always add an input time stamp to match the input sequence.
	const float CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);
	InputTimeStamps.Add(CurrentTime);

	// Prune old inputs. This would be better-suited to a ring-buffer than an array, but it's not much data.
	for (int32 i = 0; i < InputStream.Num(); ++i)
	{
		if ((InputTimeStamps[i] + InputExpirationTime) >= CurrentTime)
		{
			// Remove everything before this, then exit the loop.
			if (i > 0)
			{
				InputTimeStamps.RemoveAt(0, i, false);
				InputStream.RemoveAt(0, i * (static_cast<int32>(EFGInputButtons::Count) + 1), false);
			}
			
			break;
		}
	}

	const FFGMoveLinkToFollow MoveLinkToFollow = CurrentMove->TryLinks(this, InputStream);
	
	if (MoveLinkToFollow.SMR.CompletionType == EStateMachineCompletionType::Accepted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Switching to state %s."), *MoveLinkToFollow.Link->Move->MoveName.ToString());
		
		if (MoveLinkToFollow.Link->bClearInput ||
			MoveLinkToFollow.Link->Move->bClearInputOnEntry ||
			CurrentMove->bClearInputOnExit)
		{
			InputStream.Reset();
			InputTimeStamps.Reset();
		}
		else if (MoveLinkToFollow.SMR.DataIndex)
		{
			// Consume the input we used to get to this move.
			check((MoveLinkToFollow.SMR.DataIndex % (1 + static_cast<int32>(EFGInputButtons::Count))) == 0);
			InputTimeStamps.RemoveAt(0, MoveLinkToFollow.SMR.DataIndex / 3, false);
			InputStream.RemoveAt(0, MoveLinkToFollow.SMR.DataIndex, false);
		}

		// Set and start the new move.
		CurrentMove = MoveLinkToFollow.Link->Move;
		TimeInCurrentMove = 0.f;
		DoMove(CurrentMove);
	}
	else
	{
		TimeInCurrentMove += DeltaSeconds; // Modulate by move animation length.
	}
}

void AFGDefaultPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	InInputComponent->BindAxis("XAxis", this, &AFGDefaultPawn::ReadXAxis);
	InInputComponent->BindAxis("YAxis", this, &AFGDefaultPawn::ReadYAxis);
	InInputComponent->BindAction("LeftButton", IE_Pressed, this, &AFGDefaultPawn::LeftButtonPressed);
	InInputComponent->BindAction("LeftButton", IE_Released, this, &AFGDefaultPawn::LeftButtonReleased);
	InInputComponent->BindAction("TopButton", IE_Pressed, this, &AFGDefaultPawn::TopButtonPressed);
	InInputComponent->BindAction("TopButton", IE_Released, this, &AFGDefaultPawn::TopButtonReleased);
}

void AFGDefaultPawn::ReadXAxis(float Value)
{
	// Don't care about clamping. We just need to know negative, zero, or positive.
	DirectionInput.X = Value;
}

void AFGDefaultPawn::ReadYAxis(float Value)
{
	// Don't care about clamping. We just need to know negative, zero, or positive.
	DirectionInput.Y = Value;
}

void AFGDefaultPawn::LeftButtonPressed()
{
	ButtonsDown |= (1 << static_cast<int32>(EFGInputButtons::LeftFace));
}

void AFGDefaultPawn::LeftButtonReleased()
{
	ButtonsDown &= ~(1 << static_cast<int32>(EFGInputButtons::LeftFace));
}

void AFGDefaultPawn::TopButtonPressed()
{
	ButtonsDown |= (1 << static_cast<int32>(EFGInputButtons::TopFace));
}

void AFGDefaultPawn::TopButtonReleased()
{
	ButtonsDown &= ~(1 << static_cast<int32>(EFGInputButtons::TopFace));
}

void AFGDefaultPawn::UseGameCamera()
{
	if (AFGControlsGameMode* GM = Cast<AFGControlsGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (AFGCameraActor* Cam = Cast<AFGCameraActor>(GM->MainGameCamera))
			{
				if (UGameplayStatics::GetPlayerControllerID(PC) == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player %i registering with game camera (one)."), UGameplayStatics::GetPlayerControllerID(PC));
					Cam->PlayerOne = this;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Player %i registering with game camera (two)."), UGameplayStatics::GetPlayerControllerID(PC));
					Cam->PlayerTwo = this;
				}
				
				PC->SetViewTarget(GM->MainGameCamera);
				
				return;
			}
		}
	}
	
	// Try again next frame. Currently, there's no limit to how many times we'll do this.
	GetWorldTimerManager().SetTimerForNextTick(this, &AFGDefaultPawn::UseGameCamera);
}
