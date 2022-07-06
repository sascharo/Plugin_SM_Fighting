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
	//UPROPERTY(VisibleAnywhere, Category="Actors")
	//AActor* PlayerOne;
	//UPROPERTY(VisibleAnywhere, Category="Actors")
	//AActor* PlayerTwo;
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> Players;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	float VerticalOffset;
	
};
