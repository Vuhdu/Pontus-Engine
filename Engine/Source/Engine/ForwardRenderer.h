#pragma once

struct ID3D11DeviceContext;
struct ID3D11Buffer;

class CCamera;
class CEnvironmentLight;
class CPointLight;
class CSpotLight;
class CDirectX11Framework;
class CModelInstance;

class CForwardRenderer
{
public:
	CForwardRenderer();
	~CForwardRenderer();

	bool Init(CDirectX11Framework* aFramework);

	void SetRenderCamera(CCamera* aCamera);
	void SetEnvironmentLight(CEnvironmentLight* anEnvironmentLight);

	void Render(const std::vector<CModelInstance*>& aModelList, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>>& somePointLights, std::vector<std::pair<unsigned int, std::array<CSpotLight*, 8>>>& someSpotLights);

private:
	struct FrameBufferData
	{
		CU::Matrix4x4f myToCamera;
		CU::Matrix4x4f myToProjection;
		CU::Vector4f myCameraPosition;
		CU::Vector4f myDirectionalLightDirection;
		CU::Vector4f myDirectionalLightColor;
		unsigned int myEnvironmentLightMipCount;
		unsigned int trash[3];
	} myFrameBufferData;

	struct ObjectBufferData
	{
		CU::Matrix4x4f myToWorld;
		CU::Vector2f myUVScale;

		unsigned int myNumPointLights;
		unsigned int myNumSpotLights;

		struct PointLightData
		{
			CU::Vector4f myPosition;
			CU::Vector4f myColorAndIntensity;
			float myRange;
			CU::Vector3f myGarbage;
		} myPointLights[8];

		struct SpotLightData 
		{
			CU::Vector4f myPosition;
			CU::Vector4f myDirection;
			CU::Vector4f myColorAndIntensity;
			float myRange;
			float myInnerAngle;
			float myOuterAngle;
			float myTrash;
		} mySpotLights[8];
	} myObjectBufferData;

private:
	ID3D11Buffer* myObjectBuffer = nullptr;
	ID3D11Buffer* myFrameBuffer = nullptr;
	ID3D11DeviceContext* myContext = nullptr;

	CCamera* myRenderCamera = nullptr;
	CEnvironmentLight* myEnvironmentLight = nullptr;

};

