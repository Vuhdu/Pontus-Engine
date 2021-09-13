#pragma once
#include "FullscreenTexture.h"
#include "GBuffer.h"

enum DXGI_FORMAT;

class CFullscreenTextureFactory
{
public:
	CFullscreenTextureFactory() = default;
	~CFullscreenTextureFactory() = default;

	CFullscreenTexture CreateTexture(CU::Vector2ui aSize, DXGI_FORMAT aFormat);
	CFullscreenTexture CreateTexture(ID3D11Texture2D* aTextureTemplate);

	CFullscreenTexture CreateDepth(CU::Vector2ui aSize, DXGI_FORMAT aFormat);
	
	GBuffer CreateGBuffer(CU::Vector2ui aSize);

private:

};

