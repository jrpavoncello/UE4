// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

void UOpenDoor::OpenDoor()
{
	AActor* owner = GetOwner();

	owner->SetActorRotation(FRotator(0, OpenAngle, 0));
}

void UOpenDoor::CloseDoor()
{
	AActor* owner = GetOwner();

	owner->SetActorRotation(FRotator(0, 180, 0));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* pawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (OpenTrigger && GetTotalMassOfActorsOnPlate() > 40)
	{
		OpenDoor();
	}

	if (CloseTrigger && CloseTrigger->IsOverlappingActor(pawn))
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0;

	TArray<AActor*> OverlappingActors;
	OpenTrigger->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Overlapping Actor %s"), *Actor->GetName())
		
		UPrimitiveComponent* primitiveComponent = (UPrimitiveComponent*)Actor->GetComponentByClass<UPrimitiveComponent>();

		TotalMass += primitiveComponent->GetMass();
	}

	UE_LOG(LogTemp, Warning, TEXT("Total mass is %f"), TotalMass)

	return TotalMass;
}