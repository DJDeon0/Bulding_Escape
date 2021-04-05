// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();

	//UE_LOG(LogTemp, Warning, TEXT("Grabber component Online")); //enable this code to troubleshoot if Grabber component is working
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input Component found on %s!"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	/*else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component Not found on %s!"), *GetOwner()->GetName());
	}*/
}

void UGrabber::FindPhysicsHandle() //check for Physics Handle Component
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle component found on %s!"), *GetOwner()->GetName()); //if physics handle is not found iow Nullpointer
	}
}

void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed")); //debug
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// Try and reach any actors with physics body collision channel set
	if (ActorHit)
	{
		if (!PhysicsHandle){return;}
		//If we hit something then attach physics handle
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetPlayerReach()
		);
	}
}

void UGrabber::Release()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grabber Released")); //debug
	// Remove physics handle on release
	if (!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the physics handle is attached, move the object we are holding
	if (!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent) 
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	FHitResult Hit;
	// Ray-cast to a specific distance (reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPos(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;

	//// See what it hits
	//AActor* ActorHit = Hit.GetActor();

	//if (ActorHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	//}

	//return Hit;
}

FVector UGrabber::GetPlayerWorldPos() const // Get Player Viewpoint
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	//Logging out to test

	/*UE_LOG(LogTemp, Warning, TEXT("Player ViewPoint Location: %s Player ViewPoint Rotation: %s"),
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.ToString()
	);*/

	return PlayerViewPointLocation;

	//// Draws a line from player showing the reach
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation,
	//	LineTraceEnd,
	//	FColor(0, 255, 0),
	//	false,
	//	0.f,
	//	0,
	//	5.f
	//);
}

FVector UGrabber::GetPlayerReach() const
{
	// Get Player Viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; //Establishes the length and rotation of the line
}
