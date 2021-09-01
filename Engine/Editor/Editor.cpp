#include "pch.h"
#include "Editor.h"
#include "EditorInterface.h"

bool Editor::CEditor::Init()
{
	myEditorInterface = new CEditorInterface();

	return myEditorInterface != nullptr;
}

void Editor::CEditor::Update()
{
	
}

void Editor::CEditor::Destroy()
{
	SAFE_DELETE(myEditorInterface);
}