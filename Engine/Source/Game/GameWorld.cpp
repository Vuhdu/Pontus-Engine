#include "pch.h"
#include "GameWorld.h"

#include "ModelFactory.h"
#include "ModelInstance.h"

#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Timer.h"

void CGameWorld::Init()
{
    auto lightFactory = CEngine::GetLightFactory();

    auto light = lightFactory->CreateEnvironmentLight(L"Assets/Art/CubeMap/cube_1024_preblurred_angle3_Skansen3.dds");
    light->SetDirection({ 0.0f, 0.0f, -1.0f });
    light->SetColor({ 0.8f, 0.8f, 0.8f });

    CPointLight* redPointLight = lightFactory->CreatePointLight();
    redPointLight->SetPosition({ -200.0f, 70.0f, 500.0f });
    redPointLight->SetColor({ 1.0f, 0.0f, 0.0f });
    redPointLight->SetRange(2000.0f);
    redPointLight->SetIntensity(500000.0f);

    CPointLight* greenPointLight = lightFactory->CreatePointLight();
    greenPointLight->SetPosition({ 0.0f, 70.0f, 350.0f });
    greenPointLight->SetColor({ 0.0f, 1.0f, 0.0f });
    greenPointLight->SetRange(200.0f);
    greenPointLight->SetIntensity(500000.0f);

    CPointLight* bluePointLight = lightFactory->CreatePointLight();
    bluePointLight->SetPosition({ 200.0f, 70.0f, 350.0f });
    bluePointLight->SetColor({ 0.0f, 0.0f, 1.0f });
    bluePointLight->SetRange(200.0f);
    bluePointLight->SetIntensity(500000.0f);

    mySpotLight = lightFactory->CreateSpotLight();
    mySpotLight->SetPosition({ 0.0f, 0.0f, 0.0f });
    mySpotLight->SetColor({ 1.0f, 0.0f, 1.0f });
    mySpotLight->SetDirection({ 0.0f, 0.0f, 1.0f });
    mySpotLight->SetRange(2500.0f);
    mySpotLight->SetRadius(0.0f, 0.2f);
    mySpotLight->SetIntensity(50000.0f);

    auto chest = CEngine::GetModelFactory()->CreateModel("Chest", { 100.0f, -70.0f, 350.0f });
    myHead = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 35.0f, 350.0f });
    myHead2 = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 105.0f, 350.0f });
    auto garlicMan = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 0.0f, 35.0f, 500.0f });
}

void CGameWorld::Update(const float [[maybe_unused]] aDeltaTime)
{
    myHead->Rotate({ 0.0f, 0.01f, 0.0f });
    myHead2->Rotate({ 0.0f, -0.01f, 0.0f });

    static bool turn = true;

    auto dir = mySpotLight->GetDirection();
    if (turn)
    {
        dir.x += aDeltaTime * .25f;
        if (dir.x > 0.6f)
        {
            turn = !turn;
        }
    }
    else
    {
        dir.x -= aDeltaTime * .25f;
        if (dir.x < -0.6f)
        {
            turn = !turn;
        }
    }
    mySpotLight->SetDirection(dir);
}

void CGameWorld::Render()
{
    // Why does this exist? Nobody knows!
}
