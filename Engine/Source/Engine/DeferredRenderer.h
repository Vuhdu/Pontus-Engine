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

class CDeferredRenderer
{
public:
	CDeferredRenderer() = default;
	~CDeferredRenderer() = default;

	bool Init(CDirectX11Framework* aFramework);

	void SetRenderCamera(CCamera* aCamera);
	void SetEnvironmentLight(CEnvironmentLight* anEnvironmentLight);

	void Render(
		const std::vector<CPointLight*>& somePointLights, 
		const std::vector<CSpotLight*>& someSpotLights);

	void GenerateGBuffer(const std::vector<CModelInstance*>& aModelList);

private:
	struct FrameBufferData
	{
		CU::Matrix4x4f myToCamera;
		CU::Matrix4x4f myToProjection;
		CU::Vector4f myCameraPosition;
	} myFrameBufferData;

	struct ObjectBufferData
	{
		CU::Matrix4x4f myToWorld;
		CU::Vector2f myUVScale = { 1.0f, 1.0f };
		unsigned int padding[2];
	} myObjectBufferData;

	struct EnvironmentLightBufferData
	{
		CU::Vector4f myDirectionalLightDirection;
		CU::Vector4f myDirectionalLightColorAndIntensity;
		unsigned int myEnvironmentLightMipCount;
		unsigned int trash[3];
	} myEnvironmentLightBufferData;

	struct PointLightBufferData
	{
		CU::Vector4f myPosition;
		CU::Vector4f myColorAndIntensity;
		float myRange;
		CU::Vector3f garbage;
	} myPointLightBufferData;

	struct SpotLightBufferData
	{
		CU::Vector4f myPosition;
		CU::Vector4f myDirection;
		CU::Vector4f myColorAndIntensity;
		float myRange;
		float myInnerAngle;
		float myOuterAngle;
		float myTrash;
	} mySpotLightBufferData;

	ID3D11Buffer* myObjectBuffer = nullptr;
	ID3D11Buffer* myFrameBuffer = nullptr;
	ID3D11Buffer* myEnvironmentLightBuffer = nullptr;
	ID3D11Buffer* mySpotLightBuffer = nullptr;
	ID3D11Buffer* myPointLightBuffer = nullptr;

	ID3D11VertexShader* myVertexShader = nullptr;
	ID3D11PixelShader* myEnvironmentLightShader = nullptr;
	ID3D11PixelShader* myPointLightShader = nullptr;
	ID3D11PixelShader* mySpotLightShader = nullptr;
	ID3D11PixelShader* myGBufferShader = nullptr;

	ID3D11DeviceContext* myContext = nullptr;
	CCamera* myRenderCamera = nullptr;
	CEnvironmentLight* myEnvironmentLight = nullptr;
};

