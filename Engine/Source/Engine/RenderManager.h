#pragma once
#include "ForwardRenderer.h"
#include "FullscreenRenderer.h"

#include "FullscreenTexture.h"

class CDirectX11Framework;

class CRenderManager
{
public:
	CRenderManager() = default;
	~CRenderManager() = default;

	bool Init(CDirectX11Framework* aFramework);
	void Render();

private:
	CDirectX11Framework* myFramework = nullptr;

	CForwardRenderer myForwardRenderer = {};
	CFullscreenRenderer myFullscreenRenderer = {};

	CFullscreenTexture myBackBuffer;
	CFullscreenTexture myIntermediateDepth;
	CFullscreenTexture myIntermediateTexture;

	CFullscreenTexture myLuminanceTexture;
	CFullscreenTexture myHalfsizeTexture;
	CFullscreenTexture myQuartersizeTexture;
	CFullscreenTexture myBlurTexture1;
	CFullscreenTexture myBlurTexture2;

};