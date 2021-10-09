#pragma once
#include "WindowHandler.h"
#include "DirectX11Framework.h"
#include "RenderManager.h"

namespace Editor 
{
	class CEditor;
}

class CGraphicsEngine
{ 
public:
	CGraphicsEngine();
	~CGraphicsEngine();

	bool Init(CWindowHandler::SWindowData aWindowData);
	bool InitEditorInterface(Editor::CEditor* anEditor);

	void BeginFrame();
	void RenderFrame();
	void EndFrame();

	CDirectX11Framework* GetFramework();
	IDXGISwapChain* GetSwapChain();

private:
	CWindowHandler myWindowHandler;
	CDirectX11Framework myFrameWork;
	CRenderManager myRenderManager;

};
