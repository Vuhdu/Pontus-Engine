#pragma once
#include "ForwardRenderer.h"
#include "FullscreenRenderer.h"

#include "FullscreenTexture.h"

class CDirectX11Framework;

class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager() = default;

	bool Init(CDirectX11Framework* aFramework);
	void Render();

private:
	CForwardRenderer myForwardRenderer;
	CFullscreenRenderer myFullscreenRenderer;

	CFullscreenTexture myBackBuffer;
	CFullscreenTexture myIntermediateDepth;
	CFullscreenTexture myIntermediateTexture;

};