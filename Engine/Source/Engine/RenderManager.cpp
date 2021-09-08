#include "pch.h"
#include "RenderManager.h"
#include "DirectX11Framework.h"
#include "FullscreenTextureFactory.h"

#include "Scene.h"
#include "Camera.h"
#include "EnvironmentLight.h"

#include <d3d11.h>

bool RenderManager::Init(CDirectX11Framework* aFramework)
{
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

    return true;
}

void RenderManager::Render()
{
	myBackBuffer.ClearTexture();
	myIntermediateTexture.ClearTexture();
	myIntermediateDepth.ClearDepth();

	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

	auto scene = CEngine::GetScene();
	CCamera* renderCamera = scene->GetMainCamera();
	CEnvironmentLight* environmentLight = scene->GetEnvironmentLight();

	myForwardRenderer.SetRenderCamera(renderCamera);
	myForwardRenderer.SetEnvironmentLight(environmentLight);

	const std::vector<CModelInstance*> models = scene->CullModels();
	std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointLights;
	std::vector<std::pair<unsigned int, std::array<CSpotLight*, 8>>> spotLights;

	for (const auto& model : models)
	{
		pointLights.push_back(scene->CullPointLights(model));
		spotLights.push_back(scene->CullSpotLights(model));
	}

	myForwardRenderer.Render(models, pointLights, spotLights);
}
