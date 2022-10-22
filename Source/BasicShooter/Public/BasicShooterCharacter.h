// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"

#include "BasicShooterCharacter.generated.h"

UCLASS()
class BASICSHOOTER_API ABasicShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Val);
	void MoveSideward(float Val);
	void Fire();
	UFUNCTION(Server, Reliable)
	void ServerFire();

	void LookVertical(float Val);
	void TurnHorizontal(float Val);

	void UpdateHealthWidget();

	UFUNCTION()
	virtual void OnRep_Health();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Health)
	int32 Health;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidgetComponent;
};
