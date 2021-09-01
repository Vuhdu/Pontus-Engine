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
	CPointLight* CreatePointLight();
	CSpotLight* CreateSpotLight();

private:
	ID3D11Device* myDevice = nullptr;

};

class LightFactory
{
};

