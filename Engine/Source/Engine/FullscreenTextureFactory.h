#pragma once
#include "FullscreenTexture.h"

enum DXGI_FORMAT;

class FullscreenTextureFactory
{
public:
	FullscreenTextureFactory() = default;
	~FullscreenTextureFactory() = default;

	FullscreenTexture CreateTexture(CU::Vector2ui aSize, DXGI_FORMAT aFormat);

	FullscreenTexture CreateDepth(CU::Vector2ui aSize, DXGI_FORMAT aFormat);

private:
	FullscreenTexture CreateTexture(ID3D11Texture2D* aTextureTemplate);

};

