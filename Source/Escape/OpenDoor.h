// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

	void OpenDoor();

	void CloseDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
		float OpenAngle = 90;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* OpenTrigger;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* CloseTrigger;

	UPROPERTY(EditAnywhere)
		AActor* Instigator; // Pawn is an AActor
};