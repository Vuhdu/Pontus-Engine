#include "stdafx.h"
#include "framework.h"

#include "Engine.h"

#include "JsonParser.h"
#include "GraphicsEngine.h"

#include "ModelFactory.h"
#include "CameraFactory.h"
#include "LightFactory.h"

#include "PointLight.h"
#include "SpotLight.h"
#include "GameObject.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"

#include "Scene.h"
#include "Timer.h"
#include "InputHandler.h"

#include "Editor.h"

bool CEngine::Init() 
{
    InitConsole();
    JsonParser::GetInstance().Init();

    if (!MainSingleton::Start())
    {
        ERROR_PRINT("MainSingleton could not be instantiated!");
        return false;
    }

    CWindowHandler::SWindowData windowData;
    windowData.myX = 100;
    windowData.myY = 100;
    windowData.myWidth = 1920;
    windowData.myHeight = 1080;

    myGraphicsEngine = new CGraphicsEngine();
    if (!myGraphicsEngine->Init(windowData))
    {
        return EXIT_FAILURE;
    }
    
    myEditor = new Editor::CEditor();
    if (!myGraphicsEngine->InitEditorInterface(myEditor))
    {
        return EXIT_FAILURE;
    }

    MainSingleton::GetInstance()->InitFramework(myGraphicsEngine->GetFramework());

    auto modelFactory = MainSingleton::GetInstance()->GetModelFactory();
    modelFactory->Init(myGraphicsEngine->GetFramework()->GetDevice());

    auto chest = modelFactory->CreateModel("Chest");
    myHead = modelFactory->CreateModel("Head");
    myHead2 = modelFactory->CreateModel("Head");
    auto garlicMan = modelFactory->CreateModel("GarlicMan");

    chest->SetPosition({ 100.0f, -70.0f, 350.0f });
    myHead->SetPosition({ -100.0f, 35.0f, 350.0f });
    myHead2->SetPosition({ -100.0f, 105.0f, 350.0f });
    garlicMan->SetPosition({ 0.0f, 35.0f, 500.0f });

    CScene* scene = MainSingleton::GetInstance()->GetScene();
    scene->AddInstance(chest);
    scene->AddInstance(myHead);
    scene->AddInstance(myHead2);
    scene->AddInstance(garlicMan);

    CCameraFactory* cameraFactory = MainSingleton::GetInstance()->GetCameraFactory();
    myEditorCamera = cameraFactory->CreateCamera(90.0f);
    myEditorCamera->SetTransform({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f });
    myMainCamera = cameraFactory->CreateCamera(90.0f);
    myMainCamera->SetTransform({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f });

    scene->AddCamera(myEditorCamera);
    scene->AddCamera(myMainCamera);
    scene->SetEditorCamera(myEditorCamera);
    scene->SetMainCamera(myMainCamera);

    auto lightFactory = MainSingleton::GetInstance()->GetLightFactory();
    lightFactory->Init(myGraphicsEngine->GetFramework()->GetDevice());
    auto light = lightFactory->CreateEnvironmentLight(L"Assets/Art/CubeMap/cube_1024_preblurred_angle3_Skansen3.dds");
    light->SetDirection({ 0.0f, 0.0f, -1.0f });
    light->SetColor({ 0.8f, 0.8f, 0.8f });
    scene->AddEnvironmentLight(light);

    CPointLight* redPointLight = lightFactory->CreatePointLight();
    redPointLight->SetPosition({ -200.0f, 70.0f, 500.0f });
    redPointLight->SetColor({ 1.0f, 0.0f, 0.0f });
    redPointLight->SetRange(2000.0f);
    redPointLight->SetIntensity(500000.0f);
    scene->AddPointLight(redPointLight);

    CPointLight* greenPointLight = lightFactory->CreatePointLight();
    greenPointLight->SetPosition({ 0.0f, 70.0f, 350.0f });
    greenPointLight->SetColor({ 0.0f, 1.0f, 0.0f });
    greenPointLight->SetRange(200.0f);
    greenPointLight->SetIntensity(500000.0f);
    scene->AddPointLight(greenPointLight);


    CPointLight* bluePointLight = lightFactory->CreatePointLight();
    bluePointLight->SetPosition({ 200.0f, 70.0f, 350.0f });
    bluePointLight->SetColor({ 0.0f, 0.0f, 1.0f });
    bluePointLight->SetRange(200.0f);
    bluePointLight->SetIntensity(500000.0f);
    scene->AddPointLight(bluePointLight);

    mySpotLight = lightFactory->CreateSpotLight();
    mySpotLight->SetPosition({ 0.0f, 0.0f, 0.0f });
    mySpotLight->SetColor({ 1.0f, 0.0f, 1.0f });
    mySpotLight->SetDirection({ 0.0f, 0.0f, 1.0f });
    mySpotLight->SetRange(2500.0f);
    mySpotLight->SetRadius(0.0f, 0.2f);
    mySpotLight->SetIntensity(50000.0f);
    scene->AddSpotLight(mySpotLight);

    myInput = MainSingleton::GetInstance()->GetInput();
    myTimer = MainSingleton::GetInstance()->GetTimer();

    return true;
}

void CEngine::Update()
{
    myGraphicsEngine->BeginFrame();
    myEditor->BeginFrame();

    myTimer->Update();

    myEditorCamera->Update(myTimer->GetDeltaTime());
    myEditorCamera->HandleMovement(*myInput, myTimer->GetDeltaTime());

    
    static bool turn = true;

    auto dir = mySpotLight->GetDirection();
    if (turn)
    {
        dir.x += myTimer->GetDeltaTime() * .25f;
        if (dir.x > 0.6f)
        {
            turn = !turn;
        }
    }
    else
    {
        dir.x -= myTimer->GetDeltaTime() * .25f;
        if (dir.x < -0.6f)
        {
            turn = !turn;
        }
    }
    mySpotLight->SetDirection(dir);
    

    myMainCamera->Update(myTimer->GetDeltaTime());

    myHead->Rotate({ 0.0f, 0.01f, 0.0f });
    myHead2->Rotate({ 0.0f, -0.01f, 0.0f });

    
    myGraphicsEngine->RenderFrame();

    myEditor->EndFrame();
    myGraphicsEngine->EndFrame();
}

void CEngine::Destroy()
{
    MainSingleton::Destroy();
    CloseConsole();
}

void CEngine::InitConsole()
{
#ifndef _RELEASE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
#endif
}

void CEngine::CloseConsole()
{
#ifndef _RELEASE
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
#endif
}

/*
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    hInstance;
    hPrevInstance;
    lpCmdLine;
    nCmdShow;
    
    CGameObject* chestObj = new CGameObject("Chest");
    CGameObject* headObj = new CGameObject("Head");
    CGameObject* head2Obj = new CGameObject("Head (2)");
    CGameObject* garlicManObj = new CGameObject("GarlicMan");

    chestObj->SetModelInstance(chest);
    headObj->SetModelInstance(head);
    head2Obj->SetModelInstance(head2);
    garlicManObj->SetModelInstance(garlicMan);
    
    scene->AddGameObject(chestObj);
    scene->AddGameObject(headObj);
    scene->AddGameObject(head2Obj);
    scene->AddGameObject(garlicManObj);
    return 0;
}
*/