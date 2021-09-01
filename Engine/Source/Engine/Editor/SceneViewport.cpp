#include "pch.h"
#include "SceneViewport.h"

#include "DirectX11Framework.h"

void Editor::CSceneViewport::Draw()
{
	CViewport::Draw((void*)GetFramework()->GetEditorCameraRenderResourceView());
}
