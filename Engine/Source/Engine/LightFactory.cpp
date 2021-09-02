#include "pch.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "Inc/DDSTextureLoader.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Scene.h"

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
