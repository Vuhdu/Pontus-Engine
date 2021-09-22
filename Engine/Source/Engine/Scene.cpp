#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ParticleEmitterInstance.h"

CScene::CScene()
{
    myMainCamera = nullptr;
}

CScene::~CScene()
{
    for (auto& camera : myCameras)
    {
        delete camera;
        camera = nullptr;
    }
}

bool CScene::Init()
{
    return true;
}

void CScene::AddInstance(CModelInstance* aModelInstance)
{
    myModels.push_back(aModelInstance);
}

void CScene::AddInstance(CGameObject* aGameObject)
{
    myGameObjects.push_back(aGameObject);
}

void CScene::AddInstance(CParticleEmitterInstance* anEmitterInstance)
{
    myEmitters.push_back(anEmitterInstance);
}

std::vector<CModelInstance*>& CScene::CullModels()
{
    return myModels;
}

std::vector<CGameObject*>& CScene::GetGameObjects()
{
    return myGameObjects;
}

std::vector<CPointLight*> CScene::CullPointLights()
{
    return myPointLights;
}

std::vector<CSpotLight*> CScene::CullSpotLights()
{
    return mySpotLights;
}

std::vector<CParticleEmitterInstance*>& CScene::CullEmitters()
{
    return myEmitters;
}

void CScene::AddInstance(CEnvironmentLight* anEnvironmentLight)
{
    myEnvironmentLight = anEnvironmentLight;
}

void CScene::AddInstance(CPointLight* aPointLight)
{
    myPointLights.push_back(aPointLight);
}

void CScene::AddInstance(CSpotLight* aSpotLight)
{
    mySpotLights.push_back(aSpotLight);
}

void CScene::AddInstance(CCamera* aCamera)
{
    myCameras.push_back(aCamera);
}

void CScene::SetMainCamera(CCamera* aCamera)
{
    myMainCamera = aCamera;
}

void CScene::SetEditorCamera(CCamera* aCamera)
{
    myEditorCamera = aCamera;
}
