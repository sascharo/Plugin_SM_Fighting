#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraActor.h"
#include "FGCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class PLUGIN_SM_FIGHTING_API AFGCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	AFGCameraActor();
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY()
	AActor* PlayerOne;

	UPROPERTY()
	AActor* PlayerTwo;

	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	float VerticalOffset;
	
};
