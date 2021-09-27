#include "pch.h"
#include "ShadowRenderer.h"
#include "DirectX11Framework.h"

#include "Camera.h"


bool CShadowRenderer::Init(CDirectX11Framework* aFramework)
{
    myFramework = aFramework;

	HRESULT result;

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(SFrameBufferData);
	result = myFramework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(SObjectBufferData);
	result = myFramework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
	if (FAILED(result))
	{
		return false;
	}

    return true;
}

void CShadowRenderer::SetRenderCamera(CCamera* aCamera)
{
    myShadowCamera = aCamera;
}

void CShadowRenderer::Render(const std::vector<CModelInstance*>& aModelList)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferdata;

	myFrameBufferData.myToCamera = CU::Matrix4x4f::GetFastInverse(myShadowCamera->GetTransform());
	myFrameBufferData.myToProjection = myShadowCamera->GetProjection();

	ZeroMemory(&bufferdata, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = myFramework->GetContext()->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferdata);
	if (FAILED(result))
	{
		return;
	}
	memcpy(bufferdata.pData, &myFrameBufferData, sizeof(SFrameBufferData));
	myFramework->GetContext()->Unmap(myFrameBuffer, 0);
	myFramework->GetContext()->VSSetConstantBuffers(0, 1, &myFrameBuffer);

	// Shadow mapping branch
}
