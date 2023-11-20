



// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Logging/LogMacros.h"
#include <Kismet/GameplayStatics.h>
#include "DrawDebugHelpers.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::Move(float Value)
{
	UE_LOG(LogTemp, Display, TEXT("Tank moved forward by %f"), Value);
	// X = Value * DeltaTime * Speed
	FVector DeltaLocation(0.f);
	DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	UE_LOG(LogTemp, Display, TEXT("Tank turned forward by %f"), Value);
	// X = Value * DeltaTime * Speed
	FRotator DeltaRotation(0.f);
	DeltaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
	AddActorLocalRotation(DeltaRotation, true);
}


// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerControllerRef)
	{
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 100.f, 12, FColor::Red, false, -1.f);

		RotateTurret(HitResult.ImpactPoint);
	}

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}
