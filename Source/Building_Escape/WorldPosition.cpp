// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	//FString Log = (TEXT("Hello!")); //creating a variable that stores a string to be printed
	//FString* PrtLog = &Log; /Creating a variable that Log can use as a pointer to reference Log's data
	//PrtLog->Len(); //pointing to the length of PrtLog

	//FString ObjectNameVar = (TEXT("Name"));
	FString ObjectName = GetOwner()->GetName();

	FString ObjectPosition = GetOwner()->GetActorLocation().ToString(); //Gets Actor Location in World and converts FVector to String and stores it in ObjectPosition
	
	//FString ObjectPosition = GetOwner()->GetActorTransform().GetLocation().ToString(); //Alternative way to get the same result as above
	
	UE_LOG(LogTemp, Warning, TEXT("%s position is world is %s"), *ObjectName, *ObjectPosition); //Prints references to Unreal Output Log
	
	//UE_LOG(LogTemp, Warning, TEXT("This component is attached to %s using Overloader"), *GetOwner()->GetName()); //using overloader *(ObjectName)->(ObjectMember)

	//UE_LOG(LogTemp, Warning, TEXT("This component is attached to %s"), *ObjectName); //Prints a warning in yellow to the Unreal Engine Output Log by referencing ObjectName that points to a membervar

	
	
	
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

