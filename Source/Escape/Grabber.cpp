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

	SetupPhysicsHandleComponent();

	BindGrabInputs();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChangeGrabbedLocation();
}

void UGrabber::SetupPhysicsHandleComponent()
{
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find physics handle component for %s"), *GetOwner()->GetName())
	}
}

void UGrabber::ChangeGrabbedLocation()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		///Get player view point this tick
		FVector playerVPLocation;
		FRotator playerVPRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPLocation, playerVPRotation);

		/// Use the same starting point, but instead use the distance we calculated in the initial grab
		PhysicsHandle->SetTargetLocation(
			GetReachLineTraceEnd(playerVPLocation, playerVPRotation.Vector(), GrabbedOffsetDistance));
	}
}

void UGrabber::BindGrabInputs()
{
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

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Get player view point this tick
	FVector playerVPLocation;
	FRotator playerVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPLocation, playerVPRotation);

	FVector lineTraceEnd = GetReachLineTraceEnd(playerVPLocation, playerVPRotation.Vector(), Reach);

	/// Setup query parameters
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult hitResult;

	GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		playerVPLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);
	
	return hitResult;
}

bool UGrabber::GetCollisionByComponent(UPrimitiveComponent* componentCollided, FVector start, FVector end, FHitResult& OutHit)
{
	/// Setup query parameters
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	TArray<FHitResult> hitResult;

	GetWorld()->LineTraceMultiByObjectType(
		hitResult,
		start,
		end,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);

	for (int i = 0; i < hitResult.Num(); i++)
	{
		if (hitResult[i].GetComponent() == componentCollided)
		{
			OutHit = hitResult[i];
			return true;
		}
	}

	return false;
}

FVector UGrabber::GetReachLineTraceEnd(FVector playerVPLocation, FVector playerVPRotation, float reach)
{
	return playerVPLocation + (playerVPRotation * reach);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	/// Try and reach any actors with physics body collision channel set
	FHitResult hitResult = GetFirstPhysicsBodyInReach();

	AActor* actorHit = hitResult.GetActor();

	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("I hit %s"), *actorHit->GetName())

		UPrimitiveComponent* componentToGrab = hitResult.GetComponent();

		///Get player view point this tick
		FVector playerVPLocation;
		FRotator playerVPRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVPLocation, playerVPRotation);

		FVector lineTraceEnd = GetReachLineTraceEnd(playerVPLocation, playerVPRotation.Vector(), Reach);

		FVector componentActorLocation = componentToGrab->GetOwner()->GetActorLocation();

		/// Use the X and Y components of the object we just grabbed, but use the Z component of where we grabbed it
		FVector grabVector = FVector(componentActorLocation.X, componentActorLocation.Y, hitResult.Location.Z);

		/// Make sure we calculate the distance between the player and where we are going to grab the object by.
		/// This will be used in place of the Reach parameter during a grab.
		GrabbedOffsetDistance = (grabVector - playerVPLocation).Size();

		UE_LOG(LogTemp, Warning, TEXT("Calculated grabbed offset %f"), GrabbedOffsetDistance)

			PhysicsHandle->GrabComponent(
				componentToGrab,
				NAME_None,
				grabVector,
				true
			);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))

	PhysicsHandle->ReleaseComponent();
}

