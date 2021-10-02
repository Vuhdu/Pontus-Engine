#include "pch.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "Inc/DDSTextureLoader.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Scene.h"
#include "FullscreenTextureFactory.h"
#include "CameraFactory.h"

void CLightFactory::Init(ID3D11Device* aDevice)
{
	myDevice = aDevice;
}

CEnvironmentLight* CLightFactory::CreateEnvironmentLight(const std::wstring& aCubeMapPath)
{
	HRESULT result;

	ID3D11ShaderResourceView* shaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, aCubeMapPath.c_str(), nullptr, &shaderResourceView);
	if (FAILED(result))
	{
		assert(false && "No cube map path could be found.");
	}

	auto environmentLight = new CEnvironmentLight(shaderResourceView);

	CEngine::GetScene()->AddInstance(environmentLight);

	return environmentLight;
}

CEnvironmentLight* CLightFactory::CreateEnvironmentLight(const std::wstring& aCubeMapPath, const CU::Transform& aTransform, const CU::Vector4f& aColorAndIntensity, bool aShouldCastShadows)
{
	CEnvironmentLight* environmentLight = CreateEnvironmentLight(aCubeMapPath);

	environmentLight->SetDirection(aTransform.GetRotation());
	environmentLight->SetColor({ aColorAndIntensity.x, aColorAndIntensity.y, aColorAndIntensity.z });
	environmentLight->SetIntensity(aColorAndIntensity.w);
	environmentLight->SetShouldCastShadows(aShouldCastShadows);

	if (aShouldCastShadows)
	{
		auto shadowCamera = CEngine::GetCameraFactory()->CreateCameraWithoutAddingToScene(
			90, 
			1.0f, 10000.0f,
			CEngine::GetResolution(), 
			eOrientation::Orthographic
		);
		shadowCamera->SetPosition(aTransform.GetPosition());
		shadowCamera->SetRotation(aTransform.GetRotation());
		environmentLight->SetShadowCamera(shadowCamera);
		environmentLight->SetShadowMap(
			CEngine::GetFullscreenTextureFactory()->CreateShadowMap(CEngine::GetResolution())
		);
	}

	return environmentLight;
}

CPointLight* CLightFactory::CreatePointLight(bool aShouldCastShadows)
{
	auto pointLight = new CPointLight();
	pointLight->SetShouldRenderShadows(aShouldCastShadows);

	if (aShouldCastShadows)
	{
		for (int i = 0; i < 6; i++)
		{
			auto shadowCamera = CEngine::GetCameraFactory()->CreateCameraWithoutAddingToScene(
				90, 
				10.0f, 10000.0f,
				{ 2048, 2048 }, 
				eOrientation::Perspective
			);
			pointLight->SetShadowCameras(shadowCamera, i);
			pointLight->SetShadowMaps(
				CEngine::GetFullscreenTextureFactory()->CreateShadowMap({ 2048, 2048 }), i
			);
		}

		pointLight->GetShadowCameras()[0]->SetRotation({ 0.0f, 0.0f, 0.0f });
		pointLight->GetShadowCameras()[1]->SetRotation({ 0.0f, Utils::PI * 0.5f, 0.0f });
		pointLight->GetShadowCameras()[2]->SetRotation({ 0.0f, Utils::PI * 1.5f, 0.0f });
		pointLight->GetShadowCameras()[3]->SetRotation({ 0.0f, Utils::PI, 0.0f });
		pointLight->GetShadowCameras()[4]->SetRotation({ Utils::PI * 0.5f, 0.0f, 0.0f });
		pointLight->GetShadowCameras()[5]->SetRotation({ Utils::PI * 1.5f, 0.0f, 0.0f });
	}

	CEngine::GetScene()->AddInstance(pointLight);

	return pointLight;
}

CSpotLight* CLightFactory::CreateSpotLight(bool aShouldCastShadows)
{
	auto spotLight = new CSpotLight();

	if (aShouldCastShadows)
	{
		auto shadowCamera =
			CEngine::GetCameraFactory()->CreateCameraWithoutAddingToScene(
				90,
				10.0f, 10000.0f,
				{ 2048, 2048 },
				eOrientation::Perspective
			);
		spotLight->SetShadowCamera(shadowCamera);
		spotLight->SetShadowMap(
			CEngine::GetFullscreenTextureFactory()->CreateShadowMap({ 2048, 2048 })
		);
	}

	CEngine::GetScene()->AddInstance(spotLight);

	return spotLight;
}

