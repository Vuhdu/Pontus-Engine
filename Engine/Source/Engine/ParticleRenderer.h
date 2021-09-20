#pragma once

struct ID3D11DeviceContext;
struct ID3D11Buffer;

class CCamera;
class CEnvironmentLight;
class CPointLight;
class CSpotLight;
class CDirectX11Framework;

class CParticleEmitterInstance;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;

class CParticleRenderer
{
public:
	CParticleRenderer() = default;
	~CParticleRenderer() = default;

	bool Init(CDirectX11Framework* aFramework);

	void SetRenderCamera(CCamera* aCamera);

	void Render(const std::vector<std::shared_ptr<CParticleEmitterInstance>>& someParticleEmitters);

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

private:
	ID3D11DeviceContext* myContext = nullptr;
	CCamera* myRenderCamera = nullptr;

	ID3D11Buffer* myObjectBuffer = nullptr;
	ID3D11Buffer* myFrameBuffer = nullptr;

	//ID3D11VertexShader* myVSShader = nullptr;
	//ID3D11PixelShader* myPSShader = nullptr;
	//ID3D11GeometryShader* myGSShader = nullptr;
};

