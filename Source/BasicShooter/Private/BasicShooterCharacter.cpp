// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicShooterCharacter.h"

// Sets default values
ABasicShooterCharacter::ABasicShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ABasicShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward/Backward", this, &ABasicShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Sideward", this, &ABasicShooterCharacter::MoveSideward);

	PlayerInputComponent->BindAxis("Look Vertical", this, &ABasicShooterCharacter::LookVertical);
	PlayerInputComponent->BindAxis("Turn Horizontal", this, &ABasicShooterCharacter::TurnHorizontal);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasicShooterCharacter::Fire);
}

void ABasicShooterCharacter::MoveForward(float Val)
{
	AddMovementInput(GetActorForwardVector(), Val);
}

void ABasicShooterCharacter::MoveSideward(float Val)
{
	AddMovementInput(GetActorRightVector(), Val);
}

void ABasicShooterCharacter::Fire()
{
}

void ABasicShooterCharacter::LookVertical(float Val)
{
	AddControllerPitchInput(Val);
}

void ABasicShooterCharacter::TurnHorizontal(float Val)
{
	AddControllerYawInput(Val);
}

