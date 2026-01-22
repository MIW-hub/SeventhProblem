// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeventhProblemGameMode.h"
#include "SeventhProblemCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASeventhProblemGameMode::ASeventhProblemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
