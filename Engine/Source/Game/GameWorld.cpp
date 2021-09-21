#include "pch.h"
#include "GameWorld.h"

#include "ModelFactory.h"
#include "ModelInstance.h"

#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Timer.h"
#include "Camera.h"

CGameWorld::~CGameWorld()
{
    myLoaderThread.join();
    myLoaderThread2.join();
}

void CGameWorld::Init()
{
    auto lightFactory = CEngine::GetLightFactory();

    auto light = lightFactory->CreateEnvironmentLight(L"Assets/Art/CubeMap/cube_1024_preblurred_angle3_Skansen3.dds");
    light->SetDirection({ 0.0f, 0.0f, -1.0f });
    light->SetColor({ 0.8f, 0.8f, 0.8f });
    //InitDefaultScene(lightFactory);

    myIsMoving = true;
    myTimer = myModelLoaderTimer = 0.0f;
    myModelIndex = 0;

    auto g1 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 500.0f, -75.0f, 750.0f });
    auto g2 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 500.0f, -75.0f, 1000.0f });
    auto g3 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 500.0f, -75.0f, 1250.0f });
    auto g4 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 500.0f, -75.0f, 1500.0f });
    g1->SetRotation({ 0.0f, 90.0f, 0.0f });
    g2->SetRotation({ 0.0f, 90.0f, 0.0f });
    g3->SetRotation({ 0.0f, 90.0f, 0.0f });
    g4->SetRotation({ 0.0f, 90.0f, 0.0f });

    auto g5 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { -500.0f, -75.0f, 750.0f });
    auto g6 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { -500.0f, -75.0f, 1000.0f });
    auto g7 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { -500.0f, -75.0f, 1250.0f });
    auto g8 = CEngine::GetModelFactory()->CreateModel("GarlicMan", { -500.0f, -75.0f, 1500.0f });
    g5->SetRotation({ 0.0f, -90.0f, 0.0f });
    g6->SetRotation({ 0.0f, -90.0f, 0.0f });
    g7->SetRotation({ 0.0f, -90.0f, 0.0f });
    g8->SetRotation({ 0.0f, -90.0f, 0.0f });
}

void CGameWorld::Update(const float [[maybe_unused]] aDeltaTime)
{
    //UpdateDefaultScene(aDeltaTime);
    auto camera = CEngine::GetEditorCamera();

    if (myIsMoving)
    {
        myTimer += aDeltaTime;
        if (myTimer > 4.0f)
        {
            camera->Move({ 0.0f, 0.0f, 100.0f * aDeltaTime });

            myModelLoaderTimer += aDeltaTime;

            if (myModelLoaderTimer > 2.0f)
            {
                switch (myModelIndex)
                {
                case 0:
                    myLoaderThread = std::thread(&CGameWorld::StreamLoadModel, this, "Buddah", CU::Vector3f{ 50.0f, 35.0f, 500.0f });
                    break;
                case 1:
                    myLoaderThread.join();
                    myLoaderThread = std::thread(&CGameWorld::StreamLoadModel, this, "GarlicMan", CU::Vector3f{ 0.0f, -75.0f, 1000.0f });
                    break;
                case 2:
                {
                    myLoaderThread.join();
                    myLoaderThread = std::thread(&CGameWorld::StreamLoadModel, this, "Chest", CU::Vector3f{ 100.0f + 250.0f, -35.0f, 1750.0f });
                    myLoaderThread2 = std::thread(&CGameWorld::StreamLoadModel, this, "Chest", CU::Vector3f{ 100.0f - 250.0f, -35.0f, 1750.0f });
                } break;
                default:
                    myIsMoving = false;
                    break;
                }

                myModelLoaderTimer = 0.0f;
                myModelIndex++;
            }
        }
    }

}

void CGameWorld::Render()
{
    // Why does this exist? Nobody knows!
}

void CGameWorld::InitDefaultScene(CLightFactory* aLightFactory)
{
    CPointLight* redPointLight = aLightFactory->CreatePointLight();
    redPointLight->SetPosition({ -200.0f, 70.0f, 500.0f });
    redPointLight->SetColor({ 1.0f, 0.0f, 0.0f });
    redPointLight->SetRange(1000.0f);
    redPointLight->SetIntensity(100000.0f);

    CPointLight* greenPointLight = aLightFactory->CreatePointLight();
    greenPointLight->SetPosition({ 0.0f, 70.0f, 350.0f });
    greenPointLight->SetColor({ 0.0f, 1.0f, 0.0f });
    greenPointLight->SetRange(1000.0f);
    greenPointLight->SetIntensity(100000.0f);

    CPointLight* bluePointLight = aLightFactory->CreatePointLight();
    bluePointLight->SetPosition({ 200.0f, 70.0f, 350.0f });
    bluePointLight->SetColor({ 0.0f, 0.0f, 1.0f });
    bluePointLight->SetRange(1000.0f);
    bluePointLight->SetIntensity(100000.0f);

    mySpotLight = aLightFactory->CreateSpotLight();
    mySpotLight->SetPosition({ 0.0f, -35.0f, 0.0f });
    mySpotLight->SetColor({ 1.0f, 1.0f, 1.0f });
    mySpotLight->SetDirection({ 0.0f, 0.0f, 1.0f });
    mySpotLight->SetRange(500.0f);
    mySpotLight->SetRadius(0.0f, 0.2f);
    mySpotLight->SetIntensity(100.0f);

    myChest = CEngine::GetModelFactory()->CreateModel("Chest", { 100.0f, -70.0f, 350.0f });
    myHead = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 35.0f, 350.0f });
    myHead2 = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 105.0f, 350.0f });
    
    auto garlicMan = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 0.0f, 35.0f, 500.0f });
}

void CGameWorld::DrawSpotLightImguiMenu()
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
}

void CGameWorld::UpdateDefaultScene(const float [[maybe_unused]] aDeltaTime)
{
    DrawSpotLightImguiMenu();

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

void CGameWorld::StreamLoadModel(const char* aModelNameID, const CU::Vector3f& aPosition)
{
    CEngine::GetModelFactory()->LoadAndCreateModelFromDrive(aModelNameID, aPosition);
}
