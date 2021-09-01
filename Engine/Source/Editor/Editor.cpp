#include "pch.h"
#include "Editor.h"
#include "EditorInterface.h"

#include "WindowHandler.h"
#include "DirectX11Framework.h"

Editor::CEditor::~CEditor()
{
	SAFE_DELETE(myEditorInterface);
}

bool Editor::CEditor::Init(CWindowHandler* aWindowHandler, CDirectX11Framework* aFrameWork)
{
	myEditorInterface = new CEditorInterface();
	return myEditorInterface->Init(aWindowHandler, aFrameWork);
}

void Editor::CEditor::BeginFrame()
{
	myEditorInterface->BeginFrame();
}

void Editor::CEditor::EndFrame()
{
	myEditorInterface->EndFrame();
}
