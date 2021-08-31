#include "stdafx.h"
#include "GraphicsEngine.h"
#include "MainSingleton.h"
#include "Scene.h"
#include "PointLight.h"
#include "SpotLight.h"

CGraphicsEngine::CGraphicsEngine()
{
}

CGraphicsEngine::~CGraphicsEngine()
{
	
}

bool CGraphicsEngine::Init(CWindowHandler::SWindowData aWindowData)
{
	if (!myWindowHandler.Init(aWindowData))
	{
		return false;
	}

	if (!myFrameWork.Init(&myWindowHandler))
	{
		return false;
	}

	if (!myForwardRenderer.Init(&myFrameWork))
	{
		return false;
	}

	if (!myEditorInterface.Init(&myWindowHandler, &myFrameWork))
	{
		return false;
	}

    return true;
}

void CGraphicsEngine::BeginFrame()
{
	static auto& color = MainSingleton::GetInstance()->GetClearColor();
	myFrameWork.BeginFrame({ color.x, color.y, color.z, color.w });
	myEditorInterface.BeginFrame();
}

void CGraphicsEngine::RenderFrame()
{
	auto scene = MainSingleton::GetInstance()->GetScene();

	CEnvironmentLight* environmentLight = scene->GetEnvironmentLight();
	CCamera* editorCamera = scene->GetEditorCamera();
	CCamera* mainCamera = scene->GetMainCamera();
	std::vector<CModelInstance*> modelsToRender = scene->CullModels();
	std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointLights;
	std::vector<std::pair<unsigned int, std::array<CSpotLight*, 8>>> spotLights;

	for (CModelInstance* instance : modelsToRender)
	{
		pointLights.push_back(scene->CullPointLights(instance));
		spotLights.push_back(scene->CullSpotLights(instance));
	}

	ID3D11RenderTargetView* sceneView = myFrameWork.GetEditorCameraRenderTargetView();
	ID3D11RenderTargetView* gameView = myFrameWork.GetMainCameraRenderTargetView();

	myFrameWork.GetContext()->OMSetRenderTargets(1, &sceneView, myFrameWork.GetDepthBuffer());
	myForwardRenderer.Render(environmentLight, pointLights, spotLights, editorCamera, modelsToRender);
	myFrameWork.GetContext()->ClearDepthStencilView(myFrameWork.GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	myFrameWork.GetContext()->OMSetRenderTargets(1, &gameView, myFrameWork.GetDepthBuffer());
	myForwardRenderer.Render(environmentLight, pointLights, spotLights, mainCamera, modelsToRender);
	myFrameWork.GetContext()->ClearDepthStencilView(myFrameWork.GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	myFrameWork.GetContext()->OMSetRenderTargets(1, &myFrameWork.myBackBuffer, myFrameWork.GetDepthBuffer());
}

void CGraphicsEngine::EndFrame()
{
	myEditorInterface.EndFrame();
	myFrameWork.EndFrame();
}

CDirectX11Framework* CGraphicsEngine::GetFramework()
{
	return &myFrameWork;
}
