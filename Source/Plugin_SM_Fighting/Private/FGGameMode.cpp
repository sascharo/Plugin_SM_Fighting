#include "FGGameMode.h"
#include "FGCameraActor.h"
#include "Kismet/GameplayStatics.h"

void AFGGameMode::StartPlay()
{
	Super::StartPlay();

	if (UWorld* World = GetWorld())
	{
		MainGameCamera = Cast<AFGCameraActor>(World->SpawnActor(AFGCameraActor::StaticClass(), &FTransform::Identity));
	}
	
	// Create another player.
	if(AnotherCharacter)
	{
		if (APlayerController* PC = UGameplayStatics::CreatePlayer(this))
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				// Move this player forward (hardcoded distance, should be a UPROPERTY) and then turn around.
				Pawn->SetActorLocation(Pawn->GetActorLocation() + Pawn->GetActorForwardVector() * ActorForwardScalar);
				Pawn->AddActorLocalRotation(FRotator(Pitch, Yawn, Roll));
			}
		}
	}
}
