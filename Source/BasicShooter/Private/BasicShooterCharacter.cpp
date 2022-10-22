// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "HealthBarWidget.h"

// Sets default values
ABasicShooterCharacter::ABasicShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 100;
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetWidgetClass(UHealthBarWidget::StaticClass());
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
	// Add some recoil for realism
	AddControllerPitchInput(-0.4);

	ServerFire();
}

void ABasicShooterCharacter::ServerFire_Implementation()
{
	// Check whether we hit another player to whom we can apply damage
	FHitResult HitResult;
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	const FVector StartPos = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FVector EndPos = StartPos+PlayerController->PlayerCameraManager->GetCameraRotation().Vector()*20000;
	
	FCollisionQueryParams TraceParams("FireTrace", false, this);
	TraceParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_WorldDynamic, TraceParams);
	
	if(HitResult.IsValidBlockingHit())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit component %s from %s"), *HitResult.Component.Get()->GetName(), *HitResult.GetActor()->GetName());
		DrawDebugLine(
			GetWorld(),
			StartPos,
			HitResult.ImpactPoint,
			FColor(0, 255, 0),
			false, 5, 0,
			12.333
		);
		ABasicShooterCharacter* Character = Cast<ABasicShooterCharacter>(HitResult.Actor);
		if (Character)
		{
			if (Character->Health > 0)
			{
				int Damage = 25;
				Character->Health -= Damage;
				Character->Health = FMath::Clamp(Character->Health, 0, 100);
				UE_LOG(LogTemp, Log, TEXT("Hit character %s for damage. -> Health now at: %d"), *HitResult.GetActor()->GetName(), Character->Health);
				//UGameplayStatics::ApplyPointDamage(Character, Damage, StartPos, HitResult, this->GetOwner<APlayerController>(), this, UDamageType::StaticClass());
				Character->UpdateHealthWidget();
				if (Character->Health <= 0)
				{
					APlayerController* HitPlayerController = Cast<APlayerController>(Character->GetOwner());
					if (HitPlayerController)
						HitPlayerController->UnPossess();
					Character->RootComponent->AddLocalRotation(FQuat(FVector::RightVector, -90 * PI/180), false, nullptr, ETeleportType::ResetPhysics);
				}
			}
		}
	}
	else
	{
		DrawDebugLine(
			GetWorld(),
			StartPos,
			EndPos,
			FColor(255, 0, 0),
			false, 5, 0,
			12.333
		);
	}
}

void ABasicShooterCharacter::LookVertical(float Val)
{
	AddControllerPitchInput(Val);
}

void ABasicShooterCharacter::TurnHorizontal(float Val)
{
	AddControllerYawInput(Val);
}

void ABasicShooterCharacter::UpdateHealthWidget()
{
	UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject());
	HealthBarWidget->HealthBar->SetPercent(Health/100.f);	
}

void ABasicShooterCharacter::OnRep_Health()
{
	UpdateHealthWidget();
	UE_LOG(LogTemp, Log, TEXT("Health for %s is now: %d"), *this->GetName(), Health);
}

void ABasicShooterCharacter::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
    DOREPLIFETIME( ABasicShooterCharacter, Health );
}
