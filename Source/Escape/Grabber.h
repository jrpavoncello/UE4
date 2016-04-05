// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// How far ahead of the player can we reach
	UPROPERTY(EditAnywhere)
	float Reach = 150;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	void Grab();

	void Release();

	void SetupPhysicsHandleComponent();

	void ChangeGrabbedLocation();

	void BindGrabInputs();

	FHitResult GetFirstPhysicsBodyInReach();

	bool GetCollisionByComponent(UPrimitiveComponent* componentCollided, FVector start, FVector end, FHitResult& OutHit);

	FVector GetReachLineTraceEnd(FVector playerVPLocation, FVector playerVPRotation, float reach);

	float GrabbedOffsetDistance;

	FRotator GrabbedOffsetRotation;

	FVector GrabbedObjectOffset;
};
