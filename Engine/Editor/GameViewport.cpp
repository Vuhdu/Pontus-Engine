#include "stdafx.h"
#include "GameViewport.h"

#include "DirectX11Framework.h"

void Editor::CGameViewport::Draw()
{
	CViewport::Draw((void*)GetFramework()->GetMainCameraRenderResourceView());
}
