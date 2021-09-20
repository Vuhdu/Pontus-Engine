#pragma once
#include "Camera.h"

class CGameObject;
class CModelInstance;
class CEnvironmentLight;
class CPointLight;
class CSpotLight;
class CLight;
class CParticleEmitterInstance;

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
	void AddInstance(std::shared_ptr<CParticleEmitterInstance> anEmitterInstance);

	std::vector<CModelInstance*>& CullModels();
	std::vector<CGameObject*>& GetGameObjects();

	// To-Do: Culling method.
	std::vector<CPointLight*> CullPointLights(); 
	// To-Do: Culling method.
	std::vector<CSpotLight*> CullSpotLights();

	std::vector<std::shared_ptr<CParticleEmitterInstance>>& CullEmitters();

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
	std::vector<std::shared_ptr<CParticleEmitterInstance>> myEmitters;
};