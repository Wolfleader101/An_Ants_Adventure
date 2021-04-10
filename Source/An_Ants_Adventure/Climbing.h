// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Climbing.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AN_ANTS_ADVENTURE_API UClimbing : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbing();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Climbing")
	float Reach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float LineTraceHeight;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UInputComponent* InputComponent = nullptr;

	// setup (assumed) attached input component
	void SetupInputComponent();

	//ray-cast and grab what is in reach
	void Grab();

	// return hit for first physics body in reach
	const FHitResult GetFirstObjectInReach();

	// climbs wall
	void ClimbWall();

	// Returns current start of reach line
	FVector GetReachLineStart();

	// Returns current end of reach line
	FVector GetReachLineEnd();
		
};
