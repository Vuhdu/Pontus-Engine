#include "pch.h"
#include "Game.h"

#include "CreateParameters.h"
#include "GameWorld.h"

#include "Timer.h"

CGame::CGame(SCreateParameters* someCreateParameters)
{
	someCreateParameters->InitCallback = [this] { InitCallback(); };
	someCreateParameters->UpdateCallback = [this] { UpdateCallback(); };
	someCreateParameters->WindowResolution = { 1280, 720 };
	someCreateParameters->WindowPosition = { 100, 100 };
	someCreateParameters->ClearColor = { 0.25f, 0.75f, 0.75f, 1.0f };
	someCreateParameters->ApplicationName = L"My Awesome Game";
}

void CGame::InitCallback()
{
	myGameWorld = new CGameWorld();
	myGameWorld->Init();
}

void CGame::UpdateCallback() 
{
	const float deltaTime = CEngine::GetTimer()->GetDeltaTime();

	myGameWorld->Update(deltaTime);
	myGameWorld->Render();
}