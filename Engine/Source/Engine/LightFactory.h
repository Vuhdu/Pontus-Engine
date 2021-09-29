#pragma once

struct ID3D11Device;

class CEnvironmentLight;
class CPointLight;
class CSpotLight;

class CLightFactory
{
public:
	CLightFactory() = default;
	~CLightFactory() = default;

	void Init(ID3D11Device* aDevice);

	CEnvironmentLight* CreateEnvironmentLight(const std::wstring& aCubeMapPath);
	CEnvironmentLight* CreateEnvironmentLight(const std::wstring& aCubeMapPath, const CU::Matrix4x4f& aTransform, const CU::Vector4f& aColorAndIntensity, bool aShouldCastShadows);;
	CPointLight* CreatePointLight();
	CPointLight* CreatePointLight(const CU::Matrix4x4f& aTransform, bool aShouldCastShadows);
	CSpotLight* CreateSpotLight();

private:
	ID3D11Device* myDevice = nullptr;

};

class LightFactory
{
};

