#include "FGCameraActor.h"
#include "Camera/CameraComponent.h"

AFGCameraActor::AFGCameraActor()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	SpringArm->SetupAttachment(RootComponent);
	GetCameraComponent()->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	VerticalOffset = 150.f;
	PrimaryActorTick.bCanEverTick = true;
}

void AFGCameraActor::Tick(float DeltaSeconds)
{
	if (Players.Num() == 2)
	{
		FVector P1L = Players[0]->GetActorLocation();
		FVector P2L = Players[1]->GetActorLocation();
		
		FVector MidPoint = (P1L + P2L) * 0.5f;
		MidPoint.Z += VerticalOffset;
		SetActorLocation(MidPoint);
		
		float Pitch = FMath::RadiansToDegrees(FMath::Atan2(-VerticalOffset * 0.5f, SpringArm->TargetArmLength));
		SetActorRotation(FRotator(Pitch, FMath::RadiansToDegrees(FMath::Atan2(P1L.Y - P2L.Y, P1L.X - P2L.X)) + 90.f, 0.f));
	}
}
