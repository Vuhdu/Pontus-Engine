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
#include "FullscreenTextureFactory.h"
#include "ParticleEmitterFactory.h"

#include "ModelHandler.h"
#include "Scene.h"

CEngine::CEngine()
{
    myErrorManager = new CErrorManager();

    myTimer = new CU::Timer();
    myInput = new CU::InputHandler();

    myLightFactory = new CLightFactory();
    myModelFactory = new CModelFactory();
    myCameraFactory = new CCameraFactory();
    myFullscreenTextureFactory = new CFullscreenTextureFactory();
    myParticleEmitterFactory = new CParticleEmitterFactory();

    myModelHandler = new CModelHandler();

    myScene = new CScene();
}

CEngine::~CEngine()
{
    assert(myInstance != nullptr && "Attempted to destroy nullptr CEngine.");

    SAFE_DELETE(myScene);
    SAFE_DELETE(myModelHandler);
    SAFE_DELETE(myCameraFactory);
    SAFE_DELETE(myParticleEmitterFactory);
    SAFE_DELETE(myFullscreenTextureFactory);
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

    myWillUseEditor = myCreateParameters->UseEditorInterface;

    myClearColor = {
        myCreateParameters->ClearColor[0],
        myCreateParameters->ClearColor[1],
        myCreateParameters->ClearColor[2],
        myCreateParameters->ClearColor[3]
    };

    myResolution = {
        static_cast<unsigned int>(myCreateParameters->WindowResolution[0]),
        static_cast<unsigned int>(myCreateParameters->WindowResolution[1])
    };

    CWindowHandler::SWindowData windowData;
    windowData.myX = myCreateParameters->WindowPosition[0];
    windowData.myY = myCreateParameters->WindowPosition[1];
    windowData.myWidth = myCreateParameters->WindowResolution[0];
    windowData.myHeight = myCreateParameters->WindowResolution[1];
    windowData.myName = myCreateParameters->ApplicationName;
    windowData.myClearColor = myClearColor;

    myGraphicsEngine = new CGraphicsEngine();
    myFramework = myGraphicsEngine->GetFramework();
    
    if (!myGraphicsEngine->Init(windowData))
    {
        return EXIT_FAILURE;
    }

    myLightFactory->Init(myFramework->GetDevice());
    myModelFactory->Init(myFramework->GetDevice());
    myParticleEmitterFactory->Init(myFramework->GetDevice());

    myEditor = new Editor::CEditor();
    if (!myGraphicsEngine->InitEditorInterface(myEditor))
    {
        return EXIT_FAILURE;
    }

    CU::Transform camTransform;
    camTransform.SetPosition({ 0.0f, 0.0f, -5.0f });
    camTransform.SetRotation({ 0.0f, 0.0f, 0.0f });

    myEditorCamera = myCameraFactory->CreateCamera(90.0f, eOrientation::Perspective);
    myEditorCamera->SetTransform(camTransform);
    myMainCamera = myCameraFactory->CreateCamera(90.0f, eOrientation::Perspective);
    myMainCamera->SetTransform(camTransform);

    myScene->SetEditorCamera(myEditorCamera);
    myScene->SetMainCamera(myMainCamera);

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

        if (myInput->IsKeyDown(CU::eKeyCode::F11))
        {
            ToggleEditor(!myWillUseEditor);
        }

        myGraphicsEngine->BeginFrame();
        myEditor->BeginFrame(myWillUseEditor);

        myTimer->Update();

        myEditorCamera->Update(myTimer->GetDeltaTime());
        myEditorCamera->HandleMovement(*myInput, myTimer->GetDeltaTime());
        myMainCamera->Update(myTimer->GetDeltaTime());

        myCreateParameters->UpdateCallback();
        myGraphicsEngine->RenderFrame();

        myEditor->EndFrame(myWillUseEditor);
        myGraphicsEngine->EndFrame();

#ifndef _DEBUG
        throw std::runtime_error("error");
#endif

    }
    CloseConsole();
    return 0;
}

void CEngine::SetRenderMode(const eRenderMode& aRenderMode)
{
    myInstance->myFramework->SetRenderMode(aRenderMode);
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