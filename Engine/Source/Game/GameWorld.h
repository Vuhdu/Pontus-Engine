#pragma once

class CModelInstance;
class CSpotLight;

class CGameWorld
{
public:
	CGameWorld() = default;
	~CGameWorld() = default;

	void Init();
	void Update(const float [[maybe_unused]] aDeltaTime);
	void Render();

private:
	CModelInstance* myHead = nullptr;
	CModelInstance* myHead2 = nullptr;
	CModelInstance* myChest = nullptr;

	CSpotLight* mySpotLight = nullptr;

};

