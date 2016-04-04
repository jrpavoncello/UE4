// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"))
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	//Get player view point this tick
	FVector playerVPLocation;
	FRotator playerVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPLocation, playerVPRotation);

	UE_LOG(LogTemp, Warning, TEXT("Player Location: %s, Player Rotation: %s"),
		*playerVPLocation.ToString(), *playerVPRotation.ToString())

	FVector lineTraceEnd = playerVPLocation + (playerVPRotation.Vector() * Reach);

	DrawDebugLine(
		GetWorld(),
		playerVPLocation,
		lineTraceEnd,
		FColor(255, 0, 0),
		false,
		0,
		0,
		10
	);
}

