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
    redPointLight->SetRange(1000.0f);
    redPointLight->SetIntensity(100000.0f);

    CPointLight* greenPointLight = lightFactory->CreatePointLight();
    greenPointLight->SetPosition({ 0.0f, 70.0f, 350.0f });
    greenPointLight->SetColor({ 0.0f, 1.0f, 0.0f });
    greenPointLight->SetRange(1000.0f);
    greenPointLight->SetIntensity(100000.0f);

    CPointLight* bluePointLight = lightFactory->CreatePointLight();
    bluePointLight->SetPosition({ 200.0f, 70.0f, 350.0f });
    bluePointLight->SetColor({ 0.0f, 0.0f, 1.0f });
    bluePointLight->SetRange(1000.0f);
    bluePointLight->SetIntensity(100000.0f);

    mySpotLight = lightFactory->CreateSpotLight();
    mySpotLight->SetPosition({ 0.0f, -35.0f, 0.0f });
    mySpotLight->SetColor({ 1.0f, 1.0f, 1.0f });
    mySpotLight->SetDirection({ 0.0f, 0.0f, 1.0f });
    mySpotLight->SetRange(500.0f);
    mySpotLight->SetRadius(0.0f, 0.2f);
    mySpotLight->SetIntensity(100.0f);

    auto chest = CEngine::GetModelFactory()->CreateModel("Chest", { 100.0f, -70.0f, 350.0f });
    myHead = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 35.0f, 350.0f });
    myHead2 = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 105.0f, 350.0f });
    auto garlicMan = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 0.0f, 35.0f, 500.0f });
}

void CGameWorld::Update(const float [[maybe_unused]] aDeltaTime)
{
    if (!CEngine::IsUsingEditor())
    {
        float range = mySpotLight->GetRange();
        float innerRadius = mySpotLight->GetInnerRadius();
        float outerRadius = mySpotLight->GetOuterRadius();
        float intensity = mySpotLight->GetIntensity();
        auto spotColor = mySpotLight->GetColor();

        float color[4] = {
                spotColor.x,
                spotColor.y,
                spotColor.z,
                1.0f
        };

        ImGui::Begin("SpotLight");
        if (ImGui::SliderFloat("Inner Radius", &innerRadius, 0.0f, 1.0f))
        {
            mySpotLight->SetRadius(innerRadius, outerRadius);
        }

        if (ImGui::SliderFloat("Outer Radius", &outerRadius, 0.0f, 1.0f))
        {
            mySpotLight->SetRadius(innerRadius, outerRadius);
        }

        if (ImGui::SliderFloat("Range", &range, 0.0f, 1000.0f))
        {
            mySpotLight->SetRange(range);
        }

        if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1000.0f))
        {
            mySpotLight->SetIntensity(intensity);
        }

        if (ImGui::ColorEdit4("Color", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreviewHalf))
        {
            spotColor = { color[0], color[1], color[2] };
            mySpotLight->SetColor(spotColor);
        }

        ImGui::End();
    }

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
