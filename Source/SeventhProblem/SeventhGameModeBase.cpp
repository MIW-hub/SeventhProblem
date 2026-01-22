// Fill out your copyright notice in the Description page of Project Settings.


#include "SeventhGameModeBase.h"
#include "SeventhCharacter.h"
#include "SeventhPlayerController.h"

ASeventhGameModeBase::ASeventhGameModeBase()
{
	DefaultPawnClass = ASeventhCharacter::StaticClass();
	PlayerControllerClass = ASeventhPlayerController::StaticClass();

}