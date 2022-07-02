#include "Kismet/GameplayStatics.h"
#include "FGControlsGameMode.h"
#include "FGCameraActor.h"

void AFGControlsGameMode::StartPlay()
{
	Super::StartPlay();

	if (UWorld* World = GetWorld())
	{
		MainGameCamera = Cast<AFGCameraActor>(World->SpawnActor(AFGCameraActor::StaticClass(), &FTransform::Identity));
	}
	
	// Create another player
	if (APlayerController* PC = UGameplayStatics::CreatePlayer(this))
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			// Move this player forward (hardcoded distance, should be a UPROPERTY) and then turn around.
			Pawn->SetActorLocation(Pawn->GetActorLocation() + Pawn->GetActorForwardVector() * 150.0f);
			Pawn->AddActorLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
		}
	}
}
