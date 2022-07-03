#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FGControlsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PLUGIN_SM_FIGHTING_API AFGControlsGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

	//void SpawnSecondPlayer();

public:
	// This will be spawned when the game starts.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	ACameraActor* MainGameCamera;
	
};
