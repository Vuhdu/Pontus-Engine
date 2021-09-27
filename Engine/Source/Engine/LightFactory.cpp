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

CEnvironmentLight* CLightFactory::CreateEnvironmentLight(const std::wstring& aCubeMapPath, const CU::Matrix4x4f& aTransform, const CU::Vector4f& aColorAndIntensity, bool aShouldCastShadows)
{
	CEnvironmentLight* environmentLight = CreateEnvironmentLight(aCubeMapPath);

	environmentLight->SetDirection(aTransform.Forward());
	environmentLight->SetColor({ aColorAndIntensity.x, aColorAndIntensity.y, aColorAndIntensity.z });
	environmentLight->SetIntensity(aColorAndIntensity.w);
	environmentLight->SetShouldCastShadows(aShouldCastShadows);

	if (aShouldCastShadows)
	{
		auto shadowCamera = CEngine::GetCameraFactory()->CreateCamera(90, { 2048, 2048 }, eOrientation::Orthographic);
		shadowCamera->SetPosition(aTransform.Position());
		shadowCamera->SetRotation(aTransform.EulerAngles());
		environmentLight->SetShadowCamera(shadowCamera);
		environmentLight->SetShadowMap(
			CEngine::GetFullscreenTextureFactory()->CreateShadowMap({ 2048, 2048 })
		);
	}

	return environmentLight;
}

CPointLight* CLightFactory::CreatePointLight()
{
	auto pointLight = new CPointLight();
	
	CEngine::GetScene()->AddInstance(pointLight);
	
	return pointLight;
}

CSpotLight* CLightFactory::CreateSpotLight()
{
	auto spotLight = new CSpotLight();

	CEngine::GetScene()->AddInstance(spotLight);

	return spotLight;
}
