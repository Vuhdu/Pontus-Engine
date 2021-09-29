#include "pch.h"
#include "GameWorld.h"

#include "ParticleEmitterFactory.h"
#include "ModelFactory.h"
#include "ModelInstance.h"

#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ParticleEmitterInstance.h"

#include "Timer.h"
#include "Camera.h"
#include "Scene.h"
#include "EnvironmentLight.h"

CGameWorld::~CGameWorld()
{
    /*
    if (myLoaderThread.joinable())
    {
        myLoaderThread.join();
    }
    */
}

void CGameWorld::Init()
{
    auto lightFactory = CEngine::GetLightFactory();

    CU::Vector3f cameraPos = { 0.0f, 200.0f, -250.0f };

    CU::Matrix4x4f shadowCamera4x4f;
    shadowCamera4x4f.SetPosition(cameraPos);
    shadowCamera4x4f.SetRotationRad({ 0.5f, 0.5f, 0.0f });

    auto light = lightFactory->CreateEnvironmentLight(
        L"Assets/Art/CubeMap/cube_1024_preblurred_angle3_Skansen3.dds",
        shadowCamera4x4f,
        { 1.0f, 1.0f, 1.0f, 1.0f },
        true
    );
    light->SetDirection({ 0.0f, 0.0f, -1.0f });
    light->SetColor({ 0.8f, 0.8f, 0.8f });
    InitDefaultScene(lightFactory);

    myCameraPos = CEngine::GetModelFactory()->CreateModel("Primitive_Sphere", cameraPos);
    myCameraPos->SetScale({ 0.1f, 0.1f, 0.1f });

    /*
    myIsMoving = true;
    myTimer = 0.0f;

    bool r = true;
    for (int i = 0; i < myLoaderModels.size(); i++)
    {
        auto gr = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 500.0f, -100.0f, 750.0f + (i * 250.0f) });
        gr->SetRotation({ 0.0f, 90.0f, 0.0f });
        auto gl = CEngine::GetModelFactory()->CreateModel("GarlicMan", { -500.0f, -100.0f, 750.0f + (i * 250.0f) });
        gl->SetRotation({ 0.0f, -90.0f, 0.0f });

        myLoaderModels[i].myModelInstance = new CModelInstance();
        myLoaderModels[i].myName = (r) ? "Buddah" : "Chest";
        myLoaderModels[i].myIsLoaded = false;
        myLoaderModels[i].myDistance = 1000.0f;
        myLoaderModels[i].myModelInstance->SetPosition({ ((r) ? -60.0f : 60.0f), 25.0f, 1250 + (i * 750.0f) });

        r = !r;
    }
    */
}

void CGameWorld::Update(const float [[maybe_unused]] aDeltaTime)
{
    UpdateDefaultScene(aDeltaTime);

    ImGui::Begin("Shadow Camera");
    {
        auto environment = CEngine::GetScene()->GetEnvironmentLight();
        auto camera = environment->GetShadowCamera();
        static CU::Vector3f position = { 0.0f, 500.0f, 0.0f };
        static CU::Vector3f eulerAngles = { -25.0f, 0.0f, 0.0f };//{ 0.0f, -37.2f, -20.425f };

        ImGui::DragFloat3("position", &position.x, 1.0f, -2000.0f, 2000.0f);
        ImGui::DragFloat3("rotation", &eulerAngles.x, 0.01f, -180.0f, 180.0f);
        environment->SetDirection(eulerAngles);
        camera->SetRotation(eulerAngles);
        camera->SetPosition(position);
        myCameraPos->SetPosition(position);
    }
    ImGui::End();

    /*
    auto camera = CEngine::GetEditorCamera();

    if (myIsMoving)
    {
        myTimer += aDeltaTime;
        if (myTimer > 2.0f)
        {
            camera->Move({ 0.0f, 0.0f, 500.0f * aDeltaTime });
            for (int i = 0; i < myLoaderModels.size(); i++)
            {
                if (myLoaderModels[i].myIsLoaded == false)
                {
                    float distance = CU::Abs((camera->GetPosition() - myLoaderModels[i].myModelInstance->GetPosition()).Length());

                    if (distance < myLoaderModels[i].myDistance)
                    {
                        if (myLoaderThread.joinable())
                        {
                            myLoaderThread.join();
                        }
                        myLoaderThread = std::thread(
                            &CGameWorld::StreamLoadModel,
                            this,
                            myLoaderModels[i].myName.c_str(),
                            myLoaderModels[i].myModelInstance
                        );
                        myLoaderModels[i].myIsLoaded = true;
                    }
                }
            }
        }
    }
    */
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
    mySpotLight->SetDirection({ 0.0f, 0.0f, 0.0f });
    mySpotLight->SetRange(500.0f);
    mySpotLight->SetRadius(0.0f, 0.2f);
    mySpotLight->SetIntensity(100.0f);

    mySpotPos = CEngine::GetModelFactory()->CreateModel("Primitive_Cube", mySpotLight->GetPosition());
    

    myChest = CEngine::GetModelFactory()->CreateModel("Chest", { 100.0f, -70.0f, 350.0f });
    myHead = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 35.0f, 350.0f });
    myHead2 = CEngine::GetModelFactory()->CreateModel("Head", { -100.0f, 105.0f, 350.0f });

    myPlane = CEngine::GetModelFactory()->CreateModel("Primitive_Plane", { 0.0f, -75.0f, 350.0f });
    myPlane->SetRotation({ 3.14f * .5f, 0.0f, 0.0f });
    myPlane->SetScale({ 1.0f, 1.0f, 1.0f });

    auto garlicMan = CEngine::GetModelFactory()->CreateModel("GarlicMan", { 0.0f, 35.0f, 500.0f });

    myEmitter = CEngine::GetParticleEmitterFactory()->GetParticleEmitter("Sparks");
    myEmitter->SetPosition({ 0.0f, 0.0f, 100.0f });
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
        static CU::Vector3f position = mySpotLight->GetPosition();
        static CU::Vector3f eulerAngles = mySpotLight->GetDirection();
        static bool followCamera = false;

        if (!followCamera)
        {
            ImGui::DragFloat3("position", &position.x, 1.0f, -2000.0f, 2000.0f);
            ImGui::DragFloat3("rotation", &eulerAngles.x, 0.01f, -180.0f, 180.0f);

            mySpotLight->SetDirection({
                -eulerAngles.y,
                eulerAngles.x,
                eulerAngles.z
            });
            mySpotLight->SetPosition(position);
            
            //mySpotPos->SetRotation({ eulerAngles.x, eulerAngles.y, eulerAngles.z });
            //mySpotPos->SetPosition(position);
        }
        else
        {
            auto camera = CEngine::GetScene()->GetEditorCamera();
            eulerAngles = camera->GetTransform().Forward();
            position = camera->GetTransform().Position();
            
            mySpotLight->SetDirection({ 
                -eulerAngles.y, 
                eulerAngles.x, 
                eulerAngles.z 
            });

            mySpotLight->SetPosition(position);
            
            //mySpotPos->SetRotation({ 0, 0, 0 });
            //mySpotPos->SetPosition({ 0, 0, 0 });
        }
        ImGui::Checkbox("Follow Camera", &followCamera);

        
        mySpotPos->SetScale({ 0.25f, 0.25f, 0.25f });

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

    /*
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
    */

    myEmitter->Update(aDeltaTime, CEngine::GetEditorCamera()->GetPosition());
}

void CGameWorld::StreamLoadModel(const char* aModelNameID, CModelInstance* aModelInstance)
{
    CEngine::GetModelFactory()->LoadAndCreateModelFromDrive(aModelNameID, aModelInstance);
}
