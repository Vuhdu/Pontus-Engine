#pragma once

class CDirectX11Framework;
class CCamera;
class CModelInstance;

class ID3D11Buffer;

class CShadowRenderer
{
public:
	CShadowRenderer() = default;
	~CShadowRenderer() = default;
	
	bool Init(CDirectX11Framework* aFramework);

	void SetRenderCamera(CCamera* aCamera);

	void Render(const std::vector<CModelInstance*>& aModelList);

private:
	struct SFrameBufferData
	{
		CU::Matrix4x4f myToCamera;
		CU::Matrix4x4f myToProjection;
	} myFrameBufferData;

	struct SObjectBufferData
	{
		CU::Matrix4x4f myToWorld;
	} myObjectBufferData;

private:
	CDirectX11Framework* myFramework = nullptr;
	CCamera* myShadowCamera = nullptr;

	ID3D11Buffer* myFrameBuffer = nullptr;
	ID3D11Buffer* myObjectBuffer = nullptr;
};

