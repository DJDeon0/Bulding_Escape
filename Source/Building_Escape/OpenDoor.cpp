// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//float OpenDoor = 90.f;

	//FRotator CurrentRotation = GetOwner()->GetActorRotation();
	//FRotator OpenDoor(0.f, 90.f, 0.f);

	//GetOwner()->SetActorRotation(OpenDoor);

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + DoorAngle;

	if (!PressurePlate) //Detects if PressurePlate Component is added to an actor but not set
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it but no pressure plate set!"), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > OpenDoorMass) //If actor has PressurePlate set and is overlapping with ActorThatOpens, it will open the door
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}

	/*else
	{
		UE_LOG(LogTemp, Error, TEXT("The object %s contains the OpenDoor Component and it shouldn't."), *GetOwner()->GetName());
	}*/


	//FRotator CurrentRotation = GetOwner()->GetActorRotation();
	//CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	//OpenDoor.Yaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.02f); //old way exponential interpolation, non ideal
	//OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, 45); //Real linear interpolation
	//OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2); //Better exponential interpolation

}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("The current yaw of the door is: %f"), CurrentYaw);
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, OpenDoorRotationSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("The current yaw of the door is: %f"), CurrentYaw);
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, CloseDoorRotationSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//find all overlapping actors
	TArray<AActor*>OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}