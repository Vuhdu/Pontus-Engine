#pragma once

class CCamera;
class CEnvironmentLight;
class CPointLight;
class CSpotLight;
class CDirectX11Framework;

struct ID3D11DeviceContext;
struct ID3D11Buffer;

struct ID3D11VertexShader;
struct ID3D11PixelShader;

class CDeferredRendering
{
public:
	CDeferredRendering() = default;
	~CDeferredRendering() = default;

	bool Init(CDirectX11Framework* aFramework);

	void SetRenderCamera(CCamera* aCamera);
	void SetEnvironmentLight(CEnvironmentLight* anEnvironmentLight);

	void Render(
		std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>>& somePointLights, 
		std::vector<std::pair<unsigned int, std::array<CSpotLight*, 8>>>& someSpotLights);

	void GenerateGBuffer(const std::vector<CModelInstance*>& aModelList);

private:
	struct EnvironmentLightBufferData
	{
		CU::Vector4f myDirectionalLightDirection;
		CU::Vector4f myDirectionalLightColorAndIntensity;

		unsigned int myEnvironmentLightMipCount;
		unsigned int trash[3];
	} myEnvironmentLightBufferData;

	struct FrameBufferData
	{
		CU::Matrix4x4f myToCamera;
		CU::Matrix4x4f myToProjection;

		CU::Vector4f myCameraPosition;
	} myFrameBufferData;

	struct ObjectBufferData
	{
		CU::Matrix4x4f myToWorld;
		CU::Vector2f myUVScale;

		CU::Vector2f myTrash;
	} myObjectBufferData;

	ID3D11Buffer* myEnvironmentLightBuffer = nullptr;
	ID3D11Buffer* myObjectBuffer = nullptr;
	ID3D11Buffer* myFrameBuffer = nullptr;

	ID3D11VertexShader* myFullscreenShader = nullptr;
	ID3D11PixelShader* myEnvironmentLightShader = nullptr;

	CCamera* myRenderCamera = nullptr;
	CEnvironmentLight* myEnvironmentLight = nullptr;

	ID3D11DeviceContext* myContext = nullptr;
};

