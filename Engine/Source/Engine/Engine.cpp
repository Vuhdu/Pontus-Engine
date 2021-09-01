#include "pch.h"
#include "Engine.h"
#include "Editor.h"
#include "JsonParser.h"
#include "CreateParameters.h"
#include "GraphicsEngine.h"
#include "ForwardRenderer.h"
#include "DirectX11Framework.h"

#include "Timer.h"
#include "InputHandler.h"
#include "ErrorManager.h"

#include "ModelFactory.h"
#include "CameraFactory.h"
#include "LightFactory.h"
#include "ModelHandler.h"
#include "Scene.h"

#include "PointLight.h"
#include "SpotLight.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"

CEngine::CEngine()
{
    myErrorManager = new CErrorManager();

    myTimer = new CU::Timer();
    myInput = new CU::InputHandler();

    myLightFactory = new CLightFactory();
    myModelFactory = new CModelFactory();
    myCameraFactory = new CCameraFactory();

    myModelHandler = new CModelHandler();

    myScene = new CScene();
}

CEngine::~CEngine()
{
    assert(myInstance != nullptr && "Attempted to destroy nullptr CEngine.");

    SAFE_DELETE(myScene);
    SAFE_DELETE(myModelHandler);
    SAFE_DELETE(myCameraFactory);
    SAFE_DELETE(myModelFactory);
    SAFE_DELETE(myLightFactory);
    SAFE_DELETE(myInput);
    SAFE_DELETE(myTimer);
    SAFE_DELETE(myErrorManager);
    SAFE_DELETE(myInstance);
}

bool CEngine::Start(SCreateParameters* someCreateParameters)
{
    if (!myInstance)
    {
        myInstance = new CEngine();
        return myInstance->InternalStart(someCreateParameters);
    }
    else
    {
        assert(myInstance == nullptr && "There already exists an instance of CEngine.");
    }
    return false;
}

bool CEngine::InternalStart(SCreateParameters* someCreateParameters)
{
    InitConsole();
    JsonParser::GetInstance().Init();

    myCreateParameters = someCreateParameters;

    myClearColor = {
        myCreateParameters->ClearColor[0],
        myCreateParameters->ClearColor[1],
        myCreateParameters->ClearColor[2],
        myCreateParameters->ClearColor[3]
    };
    CWindowHandler::SWindowData windowData;
    windowData.myX = myCreateParameters->WindowPosition[0];
    windowData.myY = myCreateParameters->WindowPosition[1];
    windowData.myWidth = myCreateParameters->WindowResolution[0];
    windowData.myHeight = myCreateParameters->WindowResolution[1];
    windowData.myName = myCreateParameters->ApplicationName;
    windowData.myClearColor = myClearColor;

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

    myFramework = myGraphicsEngine->GetFramework();

    myModelFactory->Init(myGraphicsEngine->GetFramework()->GetDevice());

    auto chest = myModelFactory->CreateModel("Chest");
    myHead = myModelFactory->CreateModel("Head");
    myHead2 = myModelFactory->CreateModel("Head");
    auto garlicMan = myModelFactory->CreateModel("GarlicMan");

    chest->SetPosition({ 100.0f, -70.0f, 350.0f });
    myHead->SetPosition({ -100.0f, 35.0f, 350.0f });
    myHead2->SetPosition({ -100.0f, 105.0f, 350.0f });
    garlicMan->SetPosition({ 0.0f, 35.0f, 500.0f });

    myScene->AddInstance(chest);
    myScene->AddInstance(myHead);
    myScene->AddInstance(myHead2);
    myScene->AddInstance(garlicMan);

    myEditorCamera = myCameraFactory->CreateCamera(90.0f);
    myEditorCamera->SetTransform({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f });
    myMainCamera = myCameraFactory->CreateCamera(90.0f);
    myMainCamera->SetTransform({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f });

    myScene->AddCamera(myEditorCamera);
    myScene->AddCamera(myMainCamera);
    myScene->SetEditorCamera(myEditorCamera);
    myScene->SetMainCamera(myMainCamera);

    myLightFactory->Init(myGraphicsEngine->GetFramework()->GetDevice());
    auto light = myLightFactory->CreateEnvironmentLight(L"Assets/Art/CubeMap/cube_1024_preblurred_angle3_Skansen3.dds");
    light->SetDirection({ 0.0f, 0.0f, -1.0f });
    light->SetColor({ 0.8f, 0.8f, 0.8f });
    myScene->AddEnvironmentLight(light);

    CPointLight* redPointLight = myLightFactory->CreatePointLight();
    redPointLight->SetPosition({ -200.0f, 70.0f, 500.0f });
    redPointLight->SetColor({ 1.0f, 0.0f, 0.0f });
    redPointLight->SetRange(2000.0f);
    redPointLight->SetIntensity(500000.0f);
    myScene->AddPointLight(redPointLight);

    CPointLight* greenPointLight = myLightFactory->CreatePointLight();
    greenPointLight->SetPosition({ 0.0f, 70.0f, 350.0f });
    greenPointLight->SetColor({ 0.0f, 1.0f, 0.0f });
    greenPointLight->SetRange(200.0f);
    greenPointLight->SetIntensity(500000.0f);
    myScene->AddPointLight(greenPointLight);


    CPointLight* bluePointLight = myLightFactory->CreatePointLight();
    bluePointLight->SetPosition({ 200.0f, 70.0f, 350.0f });
    bluePointLight->SetColor({ 0.0f, 0.0f, 1.0f });
    bluePointLight->SetRange(200.0f);
    bluePointLight->SetIntensity(500000.0f);
    myScene->AddPointLight(bluePointLight);

    mySpotLight = myLightFactory->CreateSpotLight();
    mySpotLight->SetPosition({ 0.0f, 0.0f, 0.0f });
    mySpotLight->SetColor({ 1.0f, 0.0f, 1.0f });
    mySpotLight->SetDirection({ 0.0f, 0.0f, 1.0f });
    mySpotLight->SetRange(2500.0f);
    mySpotLight->SetRadius(0.0f, 0.2f);
    mySpotLight->SetIntensity(50000.0f);
    myScene->AddSpotLight(mySpotLight);

    myEngineIsRunning = true;
    return myEngineIsRunning;
}

bool CEngine::Run()
{
    if (myInstance)
    {
        return myInstance->InternalRun();
    }
    return false;
}

bool CEngine::InternalRun() 
{
    myCreateParameters->InitCallback();

    MSG windowsMessage = { 0 };

    while (myEngineIsRunning)
    {
        while (PeekMessage(&windowsMessage, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&windowsMessage);
            DispatchMessage(&windowsMessage);

            if (windowsMessage.message == WM_QUIT)
            {
                myEngineIsRunning = false;
            }
        }
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


        myCreateParameters->UpdateCallback();
        myGraphicsEngine->RenderFrame();

        myEditor->EndFrame();
        myGraphicsEngine->EndFrame();
    }
    CloseConsole();
    return 0;
}

void CEngine::SetRenderMode(const eRenderMode& aRenderMode)
{
    myFramework->SetRenderMode(aRenderMode);
}

CEngine* CEngine::GetInstance()
{
    assert(myInstance != nullptr && "No instance of CEngine found. Did you forget to call Start()?");
    return myInstance;
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