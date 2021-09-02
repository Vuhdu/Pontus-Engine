#pragma once
#include "Camera.h"

class CGameObject;
class CModelInstance;
class CEnvironmentLight;
class CPointLight;
class CSpotLight;
class CLight;

class CScene
{
public:
	CScene();
	~CScene();

	bool Init();

	void AddInstance(CModelInstance* aModelInstance);
	void AddInstance(CEnvironmentLight* anEnvironmentLight);
	void AddInstance(CPointLight* aPointLight);
	void AddInstance(CSpotLight* aSpotLight);
	void AddInstance(CCamera* aCamera);
	void AddInstance(CGameObject* aGameObject);

	std::vector<CModelInstance*>& CullModels();
	std::vector<CGameObject*>& GetGameObjects();

	std::pair<unsigned int, std::array<CPointLight*, 8>> CullPointLights(CModelInstance* aModelInstance);
	std::pair<unsigned int, std::array<CSpotLight*, 8>> CullSpotLights(CModelInstance* aModelInstance);

	void SetMainCamera(CCamera* aCamera);
	void SetEditorCamera(CCamera* aCamera);

public:
	CCamera* GetEditorCamera() { return myEditorCamera; }
	CCamera* GetMainCamera() { return myMainCamera; }
	CEnvironmentLight* GetEnvironmentLight() { return myEnvironmentLight; }

private:
	CCamera* myEditorCamera;
	CCamera* myMainCamera;
	std::vector<CCamera*> myCameras;

	std::vector<CGameObject*> myGameObjects;

	std::vector<CModelInstance*> myModels;
	CEnvironmentLight* myEnvironmentLight;

	std::vector<CPointLight*> myPointLights;
	std::vector<CSpotLight*> mySpotLights;
};