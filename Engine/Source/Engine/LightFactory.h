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
	CEnvironmentLight* CreateEnvironmentLight(const std::wstring& aCubeMapPath, const CU::Transform& aTransform, const CU::Vector4f& aColorAndIntensity, bool aShouldCastShadows);;
	CPointLight* CreatePointLight(bool aShouldCastShadows = true);
	CSpotLight* CreateSpotLight(bool aShouldCastShadows = true);

private:
	ID3D11Device* myDevice = nullptr;

};

class LightFactory
{
};

