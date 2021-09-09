#include "pch.h"
#include "RenderManager.h"
#include "DirectX11Framework.h"
#include "FullscreenTextureFactory.h"

#include "Scene.h"
#include "Camera.h"
#include "EnvironmentLight.h"

#include <d3d11.h>

bool CRenderManager::Init(CDirectX11Framework* aFramework)
{
	myFramework = aFramework;

	if (!myForwardRenderer.Init(aFramework))
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
	myBackBuffer = factory->CreateTexture(backBufferTexture);
	myIntermediateDepth = factory->CreateDepth(resolution, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT);
	myIntermediateTexture = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	
	myHalfsizeTexture = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myQuartersizeTexture = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture1 = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture2 = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	myLuminanceTexture = factory->CreateTexture(resolution, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);

    return true;
}

void CRenderManager::Render()
{
	myBackBuffer.ClearTexture();
	myIntermediateTexture.ClearTexture();
	myIntermediateDepth.ClearDepth();

	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

	auto scene = CEngine::GetScene();
	CCamera* mainCamera = scene->GetMainCamera();
	CCamera* editorCamera = scene->GetEditorCamera();
	CEnvironmentLight* environmentLight = scene->GetEnvironmentLight();

	myForwardRenderer.SetEnvironmentLight(environmentLight);

	const std::vector<CModelInstance*> models = scene->CullModels();
	std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointLights;
	std::vector<std::pair<unsigned int, std::array<CSpotLight*, 8>>> spotLights;

	for (const auto& model : models)
	{
		pointLights.push_back(scene->CullPointLights(model));
		spotLights.push_back(scene->CullSpotLights(model));
	}

	ID3D11RenderTargetView* sceneView = myFramework->GetEditorCameraRenderTargetView();
	ID3D11RenderTargetView* gameView = myFramework->GetMainCameraRenderTargetView();

	myFramework->GetContext()->OMSetRenderTargets(1, &sceneView, myFramework->GetDepthBuffer());
	myForwardRenderer.SetRenderCamera(editorCamera);
	myForwardRenderer.Render(models, pointLights, spotLights);
	myFramework->GetContext()->ClearDepthStencilView(myFramework->GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	myFramework->GetContext()->OMSetRenderTargets(1, &gameView, myFramework->GetDepthBuffer());
	myForwardRenderer.SetRenderCamera(mainCamera);
	myForwardRenderer.Render(models, pointLights, spotLights);
	myFramework->GetContext()->ClearDepthStencilView(myFramework->GetDepthBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	myFramework->GetContext()->OMSetRenderTargets(1, &myFramework->myBackBuffer, myFramework->GetDepthBuffer());

	// Luminance
	myLuminanceTexture.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::LUMINANCE);

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
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myHalfsizeTexture.SetAsResourceOnSlot(1);
	myFullscreenRenderer.Render(CFullscreenRenderer::Shader::BLOOM);
}
