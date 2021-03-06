#pragma once
#include <thread>

class CModelInstance;
class CPointLight;
class CSpotLight;
class CParticleEmitterInstance;

struct LoaderModel
{
	std::string myName;
	float myDistance = 500.0f;
	bool myIsLoaded = false;
	CModelInstance* myModelInstance = nullptr;
};

class CGameWorld
{
public:
	CGameWorld() = default;
	~CGameWorld();

	void Init();
	void Update(const float [[maybe_unused]] aDeltaTime);
	void Render();

	void InitDefaultScene(CLightFactory* aLightFactory);
	void DrawSpotLightImguiMenu();
	void UpdateDefaultScene(const float [[maybe_unused]] aDeltaTime);

	void StreamLoadModel(const char* aModelNameID, CModelInstance* aModelInstance);

private:
	CModelInstance* myHead = nullptr;
	CModelInstance* myHead2 = nullptr;
	CModelInstance* myChest = nullptr;
	CModelInstance* myPlane = nullptr;
	CModelInstance* myCameraPos = nullptr;
	CModelInstance* mySpotPos = nullptr;

	CPointLight* myPointLight = nullptr;
	CSpotLight* mySpotLight = nullptr;
	CParticleEmitterInstance* myEmitter = nullptr;

	/*
	float myTimer = 0.0f;
	bool myIsMoving = true;
	std::thread myLoaderThread;
	std::array<LoaderModel, 100> myLoaderModels;
	*/
};

