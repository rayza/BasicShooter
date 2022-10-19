// Copyright Epic Games, Inc. All Rights Reserved.


#include "BasicShooterGameModeBase.h"
#include "Public/BasicShooterCharacter.h"

ABasicShooterGameModeBase::ABasicShooterGameModeBase(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	DefaultPawnClass = ABasicShooterCharacter::StaticClass();
}
