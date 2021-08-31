#include "stdafx.h"
#include "MainSingleton.h"

#include "Timer.h"
#include "InputHandler.h"

#include "ModelFactory.h"
#include "CameraFactory.h"
#include "LightFactory.h"

#include "ErrorManager.h"

#include "ModelHandler.h"

#include "Scene.h"

#include "ForwardRenderer.h"

#include "DirectX11Framework.h"

MainSingleton::~MainSingleton()
{
    assert(myInstance != nullptr && "Attempted to destroy nullptr MainSingleton.");

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

bool MainSingleton::Start()
{
    if (!myInstance)
    {
        myInstance = new MainSingleton();
        return myInstance->InternalStart();
    }
    else 
    {
        assert(myInstance == nullptr && "There already exists an instance of MainSingleton.");
    }
    return false;
}

bool MainSingleton::InternalStart()
{
    myErrorManager = new CErrorManager();

    myTimer = new CU::Timer();
    myInput = new CU::InputHandler();

    myLightFactory = new CLightFactory();
    myModelFactory = new CModelFactory();
    myCameraFactory = new CCameraFactory();

    myModelHandler = new CModelHandler();

    myScene = new CScene();

    return true;
}

void MainSingleton::Destroy()
{
    assert(myInstance != nullptr && "Attempted to destroy nullptr MainSingleton.");
}

void MainSingleton::InitFramework(CDirectX11Framework* aFramework)
{
    myFramework = aFramework;
}

CErrorManager* MainSingleton::GetErrorManager()
{
    return myErrorManager;
}

MainSingleton* MainSingleton::GetInstance()
{
    assert(myInstance != nullptr && "No instance of MainSingleton found. Did you forget to call Start()?");
    return myInstance;
}

CModelFactory* MainSingleton::GetModelFactory()
{
    return myModelFactory;
}

CCameraFactory* MainSingleton::GetCameraFactory()
{
    return myCameraFactory;
}

CLightFactory* MainSingleton::GetLightFactory()
{
    return myLightFactory;
}

CModelHandler* MainSingleton::GetModelHandler()
{
    return myModelHandler;
}

CScene* MainSingleton::GetScene()
{
    return myScene;
}

CU::InputHandler* MainSingleton::GetInput()
{
    return myInput;
}

CU::Timer* MainSingleton::GetTimer()
{
    return myTimer;
}

CGameObject* MainSingleton::GetSelectedGameObject()
{
    return mySelectedGameObject;
}

void MainSingleton::SetSelectedGameObject(CGameObject* aGameObject)
{
    mySelectedGameObject = aGameObject;
}

void MainSingleton::SetRenderMode(const eRenderMode& aRenderMode)
{
    myFramework->SetRenderMode(aRenderMode);
}

void MainSingleton::SetClearColor(const CU::Vector4f& aClearColor)
{
    myClearColor = aClearColor;
}

const CU::Vector4f& MainSingleton::GetClearColor() const
{
    return myClearColor;
}
