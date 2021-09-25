#pragma once
#include "ForwardRenderer.h"
#include "DeferredRenderer.h"
#include "ParticleRenderer.h"
#include "FullscreenRenderer.h"

#include "FullscreenTexture.h"
#include "GBuffer.h"

class CDirectX11Framework;

struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

enum BlendState
{
	BLENDSTATE_DISABLE,
	BLENDSTATE_ALPHABLEND,
	BLENDSTATE_ADDITIVE,
	BLENDSTATE_COUNT,
};

enum DepthStencilState
{
	DEPTHSTENCILSTATE_DEFAULT,
	DEPTHSTENCILSTATE_READONLY,
	DEPTHSTENCILSTATE_COUNT,
};

enum RasterizerState
{
	RASTERIZERSTATE_DEFAULT,
	RASTERIZERSTATE_WIREFRAME,
	RASTERIZERSTATE_COUNT,
};

enum SamplerState
{
	SAMPLERSTATE_TRILINEAR,
	SAMPLERSTATE_TRILINEARWRAP,
	SAMPLERSTATE_POINT,
	SAMPLERSTATE_COUNT,
};

class CRenderManager
{
public:
	CRenderManager() = default;
	~CRenderManager() = default;

	bool Init(CDirectX11Framework* aFramework);
	void Render();

	void ClearTextures();

	void SetBlendState(BlendState aBlendState);
	void SetDepthStencilState(DepthStencilState aDepthStencilState);

private:
	void ImguiRender();
	void ForwardRender();
	void DeferredRender();
	void ParticleRender();
	void FullscreenRender();

private:
	enum class RenderMode 
	{
		Deferred,
		Forward
	} myRenderMode = RenderMode::Deferred;
	
	bool myBloomIsActive = true;

	CDirectX11Framework* myFramework = nullptr;

	CForwardRenderer myForwardRenderer = {};
	CDeferredRenderer myDeferredRenderer = {};
	CParticleRenderer myParticleRenderer = {};
	CFullscreenRenderer myFullscreenRenderer = {};
	GBuffer myGBuffer = {};

	std::array<ID3D11BlendState*, BLENDSTATE_COUNT> myBlendStates;
	std::array<ID3D11DepthStencilState*, BLENDSTATE_COUNT> myDepthStencilStates;
	std::array<ID3D11RasterizerState*, BLENDSTATE_COUNT> myRasterizerStates;
	std::array<ID3D11SamplerState*, BLENDSTATE_COUNT> mySamplerStates;

	CFullscreenTexture myDeferredTexture;

	CFullscreenTexture myBackBuffer;
	CFullscreenTexture myIntermediateDepth;
	CFullscreenTexture myIntermediateTexture;

	CFullscreenTexture myLuminanceTexture;
	CFullscreenTexture myHalfsizeTexture;
	CFullscreenTexture myQuartersizeTexture;
	CFullscreenTexture myBlurTexture1;
	CFullscreenTexture myBlurTexture2;

	// temp
	int myPass = -1;
};