#pragma once
#include "WindowHandler.h"
#include "DirectX11Framework.h"
#include "ForwardRenderer.h"
#include "EditorInterface.h"

class CGraphicsEngine
{ 
public:
	CGraphicsEngine();
	~CGraphicsEngine();

	bool Init(CWindowHandler::SWindowData aWindowData);

	void BeginFrame();
	void RenderFrame();
	void EndFrame();

	CDirectX11Framework* GetFramework();

private:
	CWindowHandler myWindowHandler;
	CDirectX11Framework myFrameWork;
	CForwardRenderer myForwardRenderer;

	Editor::CEditorInterface myEditorInterface;

};
