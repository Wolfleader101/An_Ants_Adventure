// Fill out your copyright notice in the Description page of Project Settings.


#include "Climbing.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UClimbing::UClimbing()
{
	Reach = 50.f;
	LineTraceHeight = -120.f;
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClimbing::BeginPlay()
{
	Super::BeginPlay();
	SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("Climbing Component Initiated"));	
}

/// Look for attached Input Component (only appears at run time)
void UClimbing::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		InputComponent->BindAction("Jump", IE_Repeat, this, &UClimbing::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}


// Called every frame
void UClimbing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DrawDebugLine(GetWorld(), GetReachLineStart(), GetReachLineEnd(), FColor(255, 0, 0), false, -1, 0, 15);
}

void UClimbing::Grab()
{
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstObjectInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Hitting: %s"), *ActorHit->GetName()));
		ClimbWall();
	}

}

const FHitResult UClimbing::GetFirstObjectInReach()
{
	/// Ray-cast (line trace) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		TraceParameters
	);
	return HitResult;
}
void UClimbing::ClimbWall()
{
	FVector Force = FVector(0, 0, 250);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
	CharacterMovement->Launch(Force);
}
FVector UClimbing::GetReachLineStart()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	PlayerLocation.Z += LineTraceHeight;

	return PlayerLocation;
}

FVector UClimbing::GetReachLineEnd()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FRotator PlayerRotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();
	PlayerLocation.Z += LineTraceHeight;

	return PlayerLocation + PlayerRotation.Vector() * Reach;
}

