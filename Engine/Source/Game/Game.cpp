#include "pch.h"
#include "Game.h"

#include "CreateParameters.h"

CGame::CGame(SCreateParameters* someCreateParameters)
{
	someCreateParameters->InitCallback = [this] { InitCallback(); };
	someCreateParameters->UpdateCallback = [this] { UpdateCallback(); };
	someCreateParameters->WindowResolution = { 1920, 1080 };
	someCreateParameters->WindowPosition = { 100, 100 };
	someCreateParameters->ClearColor = { 1.0f, 0.5f, 0.25f, 1.0f };
	someCreateParameters->ApplicationName = L"My Awesome Game";
}

void CGame::InitCallback()
{
	INFO_PRINT("Initialized Game");
	myUpdateHasPassed = false;
}

void CGame::UpdateCallback() 
{
	if (!myUpdateHasPassed)
	{
		INFO_PRINT("Updating Game");
		myUpdateHasPassed = true;
	}
}