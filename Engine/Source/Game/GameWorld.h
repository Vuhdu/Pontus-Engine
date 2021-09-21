#pragma once
#include <thread>

class CModelInstance;
class CSpotLight;

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

	void StreamLoadModel(const char* aModelNameID, const CU::Vector3f& aPosition);

private:
	CModelInstance* myHead = nullptr;
	CModelInstance* myHead2 = nullptr;
	CModelInstance* myChest = nullptr;

	CSpotLight* mySpotLight = nullptr;

	float myTimer = 0.0f;

	float myModelLoaderTimer = 0.0f;
	int myModelIndex = 0;
	bool myIsMoving = true;

	std::thread myLoaderThread;
	std::thread myLoaderThread2;
};

