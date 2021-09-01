#include "pch.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "Inc/DDSTextureLoader.h"
#include "PointLight.h"
#include "SpotLight.h"

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

    return new CEnvironmentLight(shaderResourceView);
}

CPointLight* CLightFactory::CreatePointLight()
{
	return new CPointLight();
}

CSpotLight* CLightFactory::CreateSpotLight()
{
	return new CSpotLight();
}
