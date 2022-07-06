#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PLUGIN_SM_FIGHTING_API AFGGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

	//void SpawnSecondPlayer();

public:
	// This will be spawned when the game starts.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	ACameraActor* MainGameCamera;
	
	// Create another player.
	UPROPERTY(EditAnywhere, Category="Players")
	bool AnotherCharacter = true;

	// Actor location.
	UPROPERTY(EditAnywhere, Category="Players")
	float ActorForwardScalar = 150.f;
	
	// Actor local rotation.
	UPROPERTY(EditAnywhere, Category="Players")
	float Pitch = 0.f;
	UPROPERTY(EditAnywhere, Category="Players")
	float Yawn = 180.f;
	UPROPERTY(EditAnywhere, Category="Players")
	float Roll = 0.f;
	
};
