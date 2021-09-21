#include "pch.h"
#include "RenderManager.h"
#include "DirectX11Framework.h"
#include "FullscreenTextureFactory.h"

#include "Scene.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "ParticleEmitterInstance.h"

#include <d3d11.h>

bool CRenderManager::Init(CDirectX11Framework* aFramework)
{
	myFramework = aFramework;
	
	if (!myForwardRenderer.Init(aFramework))
	{
		return false;
	}

	if (!myDeferredRenderer.Init(aFramework))
	{
		return false;
	}

	if (!myParticleRenderer.Init(aFramework))
	{
		return false;
	}

	if (!myFullscreenRenderer.Init())
	{
		return false;
	}

	ID3D11Resource* backBufferResource = nullptr;
	aFramework->myBackBuffer->GetResource(&backBufferResource);
	ID3D11Texture2D* backBufferTexture = reinterpret_cast<ID3D11Texture2D*>(backBufferResource);
	if (!backBufferTexture)
	{
		return false;
	}

	auto factory = CEngine::GetFullscreenTextureFactory();
	auto& resolution = CEngine::GetResolution();
	myGBuffer = factory->CreateGBuffer(resolution);

	CU::Vector2ui halfresolution = {
		static_cast<unsigned int>(CEngine::GetResolution().x / 2.0f),
		static_cast<unsigned int>(CEngine::GetResolution().y / 2.0f)
	};

	CU::Vector2ui quarterresolution = {
		static_cast<unsigned int>(halfresolution.x / 2.0f),
		static_cast<unsigned int>(halfresolution.y / 2.0f)
	};
	myBackBuffer = factory->CreateTexture(backBufferTexture);
	myDeferredTexture = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);

	myIntermediateDepth = factory->CreateDepth(resolution, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT);
	myIntermediateTexture = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	
	myHalfsizeTexture = factory->CreateTexture(halfresolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myQuartersizeTexture = factory->CreateTexture(quarterresolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture1 = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture2 = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myLuminanceTexture = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);

	// Create Blend States
	HRESULT result;
	
	D3D11_BLEND_DESC alphaBlendDesc = {};
	alphaBlendDesc.RenderTarget[0].BlendEnable = true;
	alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	alphaBlendDesc.AlphaToCoverageEnable = true;

	result = aFramework->GetDevice()->CreateBlendState(&alphaBlendDesc, &myBlendStates[BLENDSTATE_ALPHABLEND]);
	if (FAILED(result))
	{
		ERROR_PRINT("Failed to create alpha blend state.");
		return false;
	}

	D3D11_BLEND_DESC additiveBlendDesc = {};
	additiveBlendDesc.RenderTarget[0].BlendEnable = true;
	additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = aFramework->GetDevice()->CreateBlendState(&additiveBlendDesc, &myBlendStates[BLENDSTATE_ADDITIVE]);
	if (FAILED(result))
	{
		ERROR_PRINT("Failed to create additive blend state.");
		return false;
	}

	myBlendStates[BLENDSTATE_DISABLE] = nullptr;

	// Create Depth Stencil States
	D3D11_DEPTH_STENCIL_DESC readonlyDepthDesc = {};
	readonlyDepthDesc.DepthEnable = true;
	readonlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readonlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	readonlyDepthDesc.StencilEnable = false;

	result = aFramework->GetDevice()->CreateDepthStencilState(&readonlyDepthDesc, &myDepthStencilStates[DEPTHSTENCILSTATE_READONLY]);
	if (FAILED(result))
	{
		ERROR_PRINT("Failed to create depth stencil readonly state.");
		return false;
	}

	myDepthStencilStates[DEPTHSTENCILSTATE_DEFAULT] = nullptr;

	//Create Rasterizer States
	D3D11_RASTERIZER_DESC wireFrameRasterizerDesc = {};
	wireFrameRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireFrameRasterizerDesc.CullMode = D3D11_CULL_BACK;
	wireFrameRasterizerDesc.DepthClipEnable = true;

	result = aFramework->GetDevice()->CreateRasterizerState(&wireFrameRasterizerDesc, &myRasterizerStates[RASTERIZERSTATE_WIREFRAME]);
	if (FAILED(result))
	{
		ERROR_PRINT("Failed to create rasterizer wireframe state.");
		return false;
	}

	D3D11_SAMPLER_DESC pointSampleDesc = {};
	pointSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	pointSampleDesc.MinLOD = -FLT_MAX;
	pointSampleDesc.MaxLOD = FLT_MAX;

	result = aFramework->GetDevice()->CreateSamplerState(&pointSampleDesc, &mySamplerStates[SAMPLERSTATE_POINT]);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC trilWrapSampleDesc = {};
	trilWrapSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	trilWrapSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	trilWrapSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	trilWrapSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	trilWrapSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	trilWrapSampleDesc.MinLOD = -FLT_MAX;
	trilWrapSampleDesc.MaxLOD = FLT_MAX;

	result = aFramework->GetDevice()->CreateSamplerState(&trilWrapSampleDesc, &mySamplerStates[SAMPLERSTATE_TRILINEARWRAP]);
	if (FAILED(result))
	{
		return false;
	}

	mySamplerStates[SAMPLERSTATE_TRILINEAR] = nullptr;

    return true;
}

void CRenderManager::Render()
{
	ClearTextures();

	switch (myRenderMode)
	{
	case CRenderManager::RenderMode::Deferred:
		DeferredRender();
		break;

	case CRenderManager::RenderMode::Forward:
		ForwardRender();
		break;

	default:
		assert(false && "No valid render mode was found");
		break;
	}

	if (myPass == -1)
	{
		ParticleRender();
		FullscreenRender();
	}
}

void CRenderManager::ClearTextures()
{
	static CU::Vector4f clearColor = CEngine::GetClearColor();

	myBackBuffer.ClearTexture(clearColor);
	myIntermediateTexture.ClearTexture(clearColor);
	myIntermediateDepth.ClearDepth();
	myGBuffer.ClearTextures();
	myDeferredTexture.ClearTexture(clearColor);

	myHalfsizeTexture.ClearTexture(clearColor);
	myLuminanceTexture.ClearTexture(clearColor);
	myQuartersizeTexture.ClearTexture(clearColor);
	myBlurTexture1.ClearTexture(clearColor);
	myBlurTexture2.ClearTexture(clearColor);
}

void CRenderManager::SetBlendState(BlendState aBlendState)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;
	myFramework->GetContext()->OMSetBlendState(myBlendStates[aBlendState], blendFactor, sampleMask);
}

void CRenderManager::SetDepthStencilState(DepthStencilState aDepthStencilState)
{
	myFramework->GetContext()->OMSetDepthStencilState(myDepthStencilStates[aDepthStencilState], 1);
}

void CRenderManager::ForwardRender()
{
	ID3D11RenderTargetView* sceneView = myFramework->GetEditorCameraRenderTargetView();

	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

	auto scene = CEngine::GetScene();
	CCamera* editorCamera = scene->GetEditorCamera();
	CEnvironmentLight* environmentLight = scene->GetEnvironmentLight();

	myForwardRenderer.SetEnvironmentLight(environmentLight);

	const std::vector<CModelInstance*> models = scene->CullModels();
	std::vector<CPointLight*> pointLights = scene->CullPointLights();
	std::vector<CSpotLight*> spotLights = scene->CullSpotLights();

	if (CEngine::IsUsingEditor())
	{
		ID3D11RenderTargetView* gameView = myFramework->GetMainCameraRenderTargetView();
		CCamera* mainCamera = scene->GetMainCamera();

		myFramework->GetContext()->OMSetRenderTargets(1, &sceneView, myFramework->GetDepthBuffer());
		myForwardRenderer.SetRenderCamera(editorCamera);
		myForwardRenderer.Render(models, pointLights, spotLights);
		myFramework->GetContext()->ClearDepthStencilView(myFramework->GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		myFramework->GetContext()->OMSetRenderTargets(1, &gameView, myFramework->GetDepthBuffer());
		myForwardRenderer.SetRenderCamera(mainCamera);
		myForwardRenderer.Render(models, pointLights, spotLights);
		myFramework->GetContext()->ClearDepthStencilView(myFramework->GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		myForwardRenderer.SetRenderCamera(editorCamera);
		myForwardRenderer.Render(models, pointLights, spotLights);
	}

	// Luminance
	myLuminanceTexture.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::LUMINANCE);
}

void CRenderManager::DeferredRender()
{
	ID3D11RenderTargetView* sceneView = myFramework->GetEditorCameraRenderTargetView();

	auto scene = CEngine::GetScene();
	CCamera* editorCamera = scene->GetEditorCamera();
	CEnvironmentLight* environmentLight = scene->GetEnvironmentLight();

	const std::vector<CModelInstance*> models = scene->CullModels();
	const std::vector<std::shared_ptr<CParticleEmitterInstance>> emitters = scene->CullEmitters();
	std::vector<CPointLight*> pointLights = scene->CullPointLights();
	std::vector<CSpotLight*> spotLights = scene->CullSpotLights();

	myDeferredRenderer.SetEnvironmentLight(environmentLight);
	myGBuffer.SetAsActiveTarget(&myIntermediateDepth);

	if (CEngine::IsUsingEditor())
	{
		ID3D11RenderTargetView* gameView = myFramework->GetMainCameraRenderTargetView();
		CCamera* mainCamera = scene->GetMainCamera();

		myFramework->GetContext()->OMSetRenderTargets(1, &sceneView, myFramework->GetDepthBuffer());
		myDeferredRenderer.SetRenderCamera(editorCamera);
		myDeferredRenderer.GenerateGBuffer(models);
		
		SetBlendState(BLENDSTATE_ADDITIVE);
		myDeferredRenderer.Render(pointLights, spotLights);
		myFramework->GetContext()->ClearDepthStencilView(myFramework->GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		SetBlendState(BLENDSTATE_DISABLE);

		myFramework->GetContext()->OMSetRenderTargets(1, &gameView, myFramework->GetDepthBuffer());
		myDeferredRenderer.SetRenderCamera(mainCamera);
		myDeferredRenderer.GenerateGBuffer(models);
		
		SetBlendState(BLENDSTATE_ADDITIVE);
		myDeferredRenderer.Render(pointLights, spotLights);
		myFramework->GetContext()->ClearDepthStencilView(myFramework->GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		myDeferredRenderer.SetRenderCamera(editorCamera);
		myDeferredRenderer.GenerateGBuffer(models);
		
		myDeferredTexture.SetAsActiveTarget();
		myGBuffer.SetAllAsResources();
		SetBlendState(BLENDSTATE_ADDITIVE);
		myDeferredRenderer.Render(pointLights, spotLights);

		myDeferredTexture.SetAsActiveTarget(&myIntermediateDepth);
		SetDepthStencilState(DEPTHSTENCILSTATE_READONLY);
		myParticleRenderer.SetRenderCamera(editorCamera);
		myParticleRenderer.Render(emitters);

		SetDepthStencilState(DEPTHSTENCILSTATE_DEFAULT);
		SetBlendState(BLENDSTATE_DISABLE);
	}

	if (CEngine::GetInput()->IsKeyDown(CU::eKeyCode::F6))
	{
		myPass++;
		if (myPass >= static_cast<int>(GBuffer::GBufferTexture::COUNT))
		{
			myPass = -1;
		}
	}

	if (myPass == -1)
	{
		// Luminance
		myLuminanceTexture.SetAsActiveTarget();
		myDeferredTexture.SetAsResourceOnSlot(0);
		myFullscreenRenderer.Render(CFullscreenRenderer::Shader::LUMINANCE);
	}
	else
	{
		//if (static_cast<GBuffer::GBufferTexture>(myPass) >= GBuffer::GBufferTexture::AMBIENTOCCLUSION)
		//{
		//	// To-do: add copy shader (grey scale)
		//}
		//else
		//{
			myBackBuffer.SetAsActiveTarget();
			myGBuffer.SetAsResourceOnSlot(static_cast<GBuffer::GBufferTexture>(myPass), 0);
			myFullscreenRenderer.Render(CFullscreenRenderer::Shader::COPY);
		//}
	}
}

void CRenderManager::ParticleRender()
{
}

void CRenderManager::FullscreenRender()
{
	// Downsamples
	myHalfsizeTexture.SetAsActiveTarget();
	myLuminanceTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::COPY);

	myQuartersizeTexture.SetAsActiveTarget();
	myHalfsizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::COPY);

	myBlurTexture1.SetAsActiveTarget();
	myQuartersizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::COPY);

	// Gaussian Blurs
	myBlurTexture2.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::GAUSSIANHORIZONTAL);

	myBlurTexture1.SetAsActiveTarget();
	myBlurTexture2.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::GAUSSIANVERTICAL);

	myBlurTexture2.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::GAUSSIANHORIZONTAL);

	myBlurTexture1.SetAsActiveTarget();
	myBlurTexture2.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::GAUSSIANVERTICAL);

	// Bloom
	myQuartersizeTexture.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::COPY);

	myHalfsizeTexture.SetAsActiveTarget();
	myQuartersizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::COPY);

	myBackBuffer.SetAsActiveTarget();

	switch (myRenderMode)
	{
	case CRenderManager::RenderMode::Deferred:
		myDeferredTexture.SetAsResourceOnSlot(0);
		break;
	case CRenderManager::RenderMode::Forward:
		myIntermediateTexture.SetAsResourceOnSlot(0);
		break;
	default:
		break;
	}
	myHalfsizeTexture.SetAsResourceOnSlot(1);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::BLOOM);
}
