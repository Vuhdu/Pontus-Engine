#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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

std::vector<CModelInstance*>& CScene::CullModels()
{
    return myModels;
}

std::vector<CGameObject*>& CScene::GetGameObjects()
{
    return myGameObjects;
}

std::pair<unsigned int, std::array<CPointLight*, 8>> CScene::CullPointLights(CModelInstance* aModelInstance)
{
    std::array<CPointLight*, 8> pointlights = { nullptr };
    unsigned int index = 0;

    for (auto light : myPointLights)
    {
        auto lightPos = light->GetPosition();
        auto instancePos = aModelInstance->GetPosition();
        auto squaredDistance = (lightPos - instancePos).LengthSqr();
        auto squaredRange = light->GetRange() * light->GetRange();
        if (squaredDistance < squaredRange)
        {
            pointlights[index] = light;
            index++;
        }
    }

    std::pair<unsigned int, std::array<CPointLight*, 8>> result;
    result.first = index;
    result.second = pointlights;

    return result;
}


std::pair<unsigned int, std::array<CSpotLight*, 8>> CScene::CullSpotLights(CModelInstance* aModelInstance)
{
    std::array<CSpotLight*, 8> spotlights = { nullptr };
    unsigned int index = 0;

    for (auto light : mySpotLights)
    {
        auto lightPos = light->GetPosition();
        auto instancePos = aModelInstance->GetPosition();
        auto squaredDistance = (lightPos - instancePos).LengthSqr();
        auto squaredRange = light->GetRange() * light->GetRange();
        if (squaredDistance < squaredRange)
        {
            spotlights[index] = light;
            index++;
        }
    }

    std::pair<unsigned int, std::array<CSpotLight*, 8>> result;
    result.first = index;
    result.second = spotlights;

    return result;
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
