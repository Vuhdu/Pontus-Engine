#include "pch.h"
#include "GraphicsEngine.h"
#include "Scene.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Editor.h"

#include <d3d11.h>

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

	if (!myRenderManager.Init(&myFrameWork))
	{
		return false;
	}

    return true;
}

bool CGraphicsEngine::InitEditorInterface(Editor::CEditor* anEditor)
{
	if (!anEditor->Init(&myWindowHandler, &myFrameWork))
	{
		return false;
	}
	return true;
}

void CGraphicsEngine::BeginFrame()
{
	static auto& color = CEngine::GetClearColor();
	myFrameWork.BeginFrame({ color.x, color.y, color.z, color.w });
}

void CGraphicsEngine::RenderFrame()
{
	myRenderManager.Render();
}

void CGraphicsEngine::EndFrame()
{
	myFrameWork.EndFrame();
}

CDirectX11Framework* CGraphicsEngine::GetFramework()
{
	return &myFrameWork;
}

IDXGISwapChain* CGraphicsEngine::GetSwapChain()
{
	return myFrameWork.GetSwapChain();
}
