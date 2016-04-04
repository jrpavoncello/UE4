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

	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find physics handle component for %s"), *GetOwner()->GetName())
	}

	/// Look for attached input component (only available at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find input component for %s"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	///Get player view point this tick
	FVector playerVPLocation;
	FRotator playerVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPLocation, playerVPRotation);

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

	/// Setup query parameters
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult hitResult;
	
	if (GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		playerVPLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	))
	{
		AActor* actorHit = hitResult.GetActor();

		UE_LOG(LogTemp, Warning, TEXT("I hit %s"), *actorHit->GetName())
	}
}

