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

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* pawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (OpenTrigger && GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpenRequest.Broadcast();
	}
	else
	{
		OnCloseRequest.Broadcast();
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
		
		UPrimitiveComponent* primitiveComponent = (UPrimitiveComponent*)Actor->FindComponentByClass<UPrimitiveComponent>();

		TotalMass += primitiveComponent->GetMass();
	}

	UE_LOG(LogTemp, Warning, TEXT("Total mass is %f"), TotalMass)

	return TotalMass;
}