#include "stdafx.h"
#include "Viewport.h"

#include "MainSingleton.h"
#include "WindowHandler.h"
#include "DirectX11Framework.h"

void Editor::CViewport::SetFramework(CDirectX11Framework* aFrameWork)
{
	myFrameWork = aFrameWork;
}

void Editor::CViewport::SetWindowHandler(CWindowHandler* aWindowHandler)
{
	myWindowHandler = aWindowHandler;
}

void Editor::CViewport::Draw()
{
	// Do nothing, call on Draw(void*) instead in child-class
}

void Editor::CViewport::Draw(void* aVoidPtr)
{
	if (!aVoidPtr)
	{
		return;
	}

	const ImVec2 imageOffset = { 8.0f, 25.0f };	// Some weird offset required to get image completely centered

	const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	const ImVec2 imageSize = ClampToAspectRatio(viewportPanelSize, { 16, 9 });

	ImVec2 pos = {
		(viewportPanelSize.x - imageSize.x) * .5f,
		(viewportPanelSize.y - imageSize.y) * .5f
	};
	pos.x += imageOffset.x;
	pos.y += imageOffset.y;
	ImGui::SetCursorPos(pos);

	ImGui::Image(aVoidPtr, imageSize);
}

const ImVec2 Editor::CViewport::ClampToAspectRatio(const ImVec2& aSizeToScale, const ImVec2& anAspectRatio) const
{
	float x = aSizeToScale.x, y = aSizeToScale.y;

	if (aSizeToScale.x < aSizeToScale.y * (anAspectRatio.x / anAspectRatio.y))
	{
		y = aSizeToScale.x * (anAspectRatio.y / anAspectRatio.x);
		x = aSizeToScale.x;
	}
	else if (aSizeToScale.y < aSizeToScale.x * (anAspectRatio.y / anAspectRatio.x))
	{
		y = aSizeToScale.y;
		x = aSizeToScale.y * (anAspectRatio.x / anAspectRatio.y);
	}
	else
	{
		y = aSizeToScale.y;
		x = aSizeToScale.x;
	}
	return ImVec2(x, y);
}

CDirectX11Framework* Editor::CViewport::GetFramework() const
{
	return myFrameWork;
}
