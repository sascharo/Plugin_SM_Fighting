#include "FGDefaultPawn.h"

#include <string>

#include "FGGameMode.h"
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
//	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	Super::BeginPlay();

	if (UStaticMeshComponent* SMC = GetMeshComponent())
	{
		SMC->SetHiddenInGame(true);
	}

	if (!CurrentMove)
	{
		UE_LOG(LogTemp, Error, TEXT("No initial move!"));
		Destroy();
		
		return;
	}
	// Check that we have all the states we need.
	if (!DirectionDownBackAtom || !DirectionDownAtom || !DirectionDownForwardAtom ||
		!DirectionBackAtom || !DirectionNeutralAtom || !DirectionForwardAtom ||
		!DirectionUpBackAtom || !DirectionUpAtom || !DirectionUpForwardAtom)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing at least one directional atom!"));
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
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
	Super::Tick(DeltaSeconds);

	// Process input.

	//const auto NegDirectionThreshold = -DirectionThreshold;
	
	// Add one atom for stick direction.
	UFGDirectionalInputAtom* FGDirectionalInputAtom = nullptr;
	
	if (DirectionInput.X < -DirectionThreshold)
	{
		//UE_LOG(LogTemp, Warning, TEXT("DirectionInput.X = %f"), DirectionInput.X);
		
		if (DirectionInput.Y < -DirectionThreshold)
		{
			FGDirectionalInputAtom = DirectionDownBackAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			FGDirectionalInputAtom = DirectionBackAtom;
		}
		else
		{
			FGDirectionalInputAtom = DirectionUpBackAtom;
		}
	}
	else if (DirectionInput.X < DirectionThreshold)
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			FGDirectionalInputAtom = DirectionDownAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			FGDirectionalInputAtom = DirectionNeutralAtom;
		}
		else
		{
			FGDirectionalInputAtom = DirectionUpAtom;
		}
	}
	else
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			FGDirectionalInputAtom = DirectionDownForwardAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			FGDirectionalInputAtom = DirectionForwardAtom;
		}
		else
		{
			FGDirectionalInputAtom = DirectionUpForwardAtom;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("FGDirectionalInputAtom = %s"), *FGDirectionalInputAtom->Description.ToString());
	InputStream.Add(FGDirectionalInputAtom);

	UE_LOG(LogTemp, Warning, TEXT("FGInputButtons::Count = %i | ButtonsDown = %i"), EFGInputButtons::Count, ButtonsDown);
	// Add one atom for each button's state.
	for (int32 i = 0; i < (int32)EFGInputButtons::Count; ++i)
	{
		if (ButtonsDown & 1<<i)
		{
			if (ButtonsDown_Old & 1<<i)
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
	UE_LOG(LogTemp, Warning, TEXT("InputStream.Num() = %i"), InputStream.Num());

	// Cache old button state so we can distinguish between held and just pressed.
	ButtonsDown_Old = ButtonsDown;
	
	// Always add an input time stamp to match the input sequence.
	const float CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);
	InputTimeStamps.Add(CurrentTime);

	// Prune old inputs. This would be better-suited to a ring-buffer than an array, but it's not much data.
	for (int32 i = 0; i < InputStream.Num(); ++i)
	{
		if (InputTimeStamps[i] + InputExpirationTime >= CurrentTime)
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

	FFGMoveLinkToFollow MoveLinkToFollow = CurrentMove->TryLinks(this, InputStream);
	//UE_LOG(LogTemp, Warning, TEXT("State: %s"), *CurrentMove->MoveName.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("State: %hhd"), static_cast<uint32>(MoveLinkToFollow.SMResult.CompletionType));
	
	if (MoveLinkToFollow.SMResult.CompletionType == EStateMachineCompletionType::Accepted)
	{
		UE_LOG(LogTemp, Error, TEXT("...........................................................................\n...........................................................................\n..........................................................................."));
		UE_LOG(LogTemp, Warning, TEXT("Switching to state %s."), *MoveLinkToFollow.Link->Move->Name.ToString());
		
		if (MoveLinkToFollow.Link->bClearInput ||
			MoveLinkToFollow.Link->Move->bClearInputOnEntry ||
			CurrentMove->bClearInputOnExit)
		{
			InputStream.Reset();
			InputTimeStamps.Reset();
		}
		else if (MoveLinkToFollow.SMResult.DataIndex)
		{
			// Consume the input we used to get to this move.
			check(MoveLinkToFollow.SMResult.DataIndex % (1 + static_cast<int32>(EFGInputButtons::Count)) == 0);
			InputTimeStamps.RemoveAt(0, MoveLinkToFollow.SMResult.DataIndex / (1 + static_cast<int32>(EFGInputButtons::Count)), false); // / 3
			InputStream.RemoveAt(0, MoveLinkToFollow.SMResult.DataIndex, false);
		}

		// Set and start the new move.
		CurrentMove = MoveLinkToFollow.Link->Move;
		TimeInCurrentMove = 0.f;
		UE_LOG(LogTemp, Warning, TEXT("Attempting to do move %s"), CurrentMove ? *CurrentMove->Name.ToString() : TEXT("NULL!"));
		DoMove(CurrentMove);
	}
	else
	{
		TimeInCurrentMove += DeltaSeconds; // Modulate by move animation length.
//		UE_LOG(LogTemp, Warning, TEXT("..."));
	}
}

void AFGDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));

	// Action mappings.
	PlayerInputComponent->BindAction("LeftButton", IE_Pressed, this, &AFGDefaultPawn::LeftButtonPressed);
	PlayerInputComponent->BindAction("LeftButton", IE_Released, this, &AFGDefaultPawn::LeftButtonReleased);
	PlayerInputComponent->BindAction("TopButton", IE_Pressed, this, &AFGDefaultPawn::TopButtonPressed);
	PlayerInputComponent->BindAction("TopButton", IE_Released, this, &AFGDefaultPawn::TopButtonReleased);

	// Axis mappings.
	PlayerInputComponent->BindAxis("XAxis", this, &AFGDefaultPawn::ReadXAxis);
	PlayerInputComponent->BindAxis("YAxis", this, &AFGDefaultPawn::ReadYAxis);
}

void AFGDefaultPawn::LeftButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftButton Pressed"));
	ButtonsDown |= (1 << static_cast<int32>(EFGInputButtons::LeftFace));
}

void AFGDefaultPawn::LeftButtonReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftButton Released"));
	ButtonsDown &= ~(1 << static_cast<int32>(EFGInputButtons::LeftFace));
}

void AFGDefaultPawn::TopButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("TopButton Pressed"));
	ButtonsDown |= (1 << static_cast<int32>(EFGInputButtons::TopFace));
}

void AFGDefaultPawn::TopButtonReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("TopButton Released"));
	ButtonsDown &= ~(1 << static_cast<int32>(EFGInputButtons::TopFace));
}

void AFGDefaultPawn::ReadXAxis(float Value)
{
//	UE_LOG(LogTemp, Warning, TEXT("XAxis"));
	// Don't care about clamping. We just need to know negative, zero, or positive.
	DirectionInput.X = Value;
}

void AFGDefaultPawn::ReadYAxis(float Value)
{
//	UE_LOG(LogTemp, Warning, TEXT("YAxis"));
	// Don't care about clamping. We just need to know negative, zero, or positive.
	DirectionInput.Y = Value;
}

void AFGDefaultPawn::UseGameCamera()
{
	/*AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
	
	if (GameModeBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameModeBase"));

		const AFGControlsGameMode* FGControlsGameMode = Cast<AFGControlsGameMode>(GameModeBase);

		if (FGControlsGameMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("FGControlsGameMode"));
		}
	}*/
	
	if (AFGGameMode* GM = Cast<AFGGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			EnableInput(PC);
			
			if (AFGCameraActor* FGCameraActor = Cast<AFGCameraActor>(GM->MainGameCamera))
			{
				if (UGameplayStatics::GetPlayerControllerID(PC) == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player %i registering with game camera (one)."), UGameplayStatics::GetPlayerControllerID(PC));
					//FGCameraActor->PlayerOne = this;
					FGCameraActor->Players.Add(this);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Player %i registering with game camera (two)."), UGameplayStatics::GetPlayerControllerID(PC));
					//FGCameraActor->PlayerTwo = this;
					FGCameraActor->Players.Add(this);
				}
				
				PC->SetViewTarget(GM->MainGameCamera);
				
				return;
			}
		}
	}
	
	// Try again next frame. Currently, there's no limit to how many times we'll do this.
	GetWorldTimerManager().SetTimerForNextTick(this, &AFGDefaultPawn::UseGameCamera);
}
