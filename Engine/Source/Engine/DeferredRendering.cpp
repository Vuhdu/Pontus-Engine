#include "pch.h"
#include "DeferredRendering.h"
#include "DirectX11Framework.h"

#include "Camera.h"
#include "EnvironmentLight.h"

#include "Model.h"
#include "ModelInstance.h"

#include <d3d11.h>

bool CDeferredRendering::Init(CDirectX11Framework* aFramework)
{
	myContext = aFramework->GetContext();
	if (!myContext)
	{
		return false;
	}

	ID3D11Device* device = aFramework->GetDevice();
	if (!device)
	{
		return false;
	}
	
	HRESULT result;

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	result = device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CDeferredRendering::SetRenderCamera(CCamera* aCamera)
{
	myRenderCamera = aCamera;
}

void CDeferredRendering::SetEnvironmentLight(CEnvironmentLight* anEnvironmentLight)
{
	myEnvironmentLight = anEnvironmentLight;
}

void CDeferredRendering::Render(std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>>& somePointLights, std::vector<std::pair<unsigned int, std::array<CSpotLight*, 8>>>& someSpotLights)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myEnvironmentLightBufferData.myDirectionalLightDirection = myEnvironmentLight->GetDirection();
	myEnvironmentLightBufferData.myDirectionalLightColorAndIntensity = myEnvironmentLight->GetColor();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = myContext->Map(myEnvironmentLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	assert(SUCCEEDED(result));
	memcpy(bufferData.pData, &myEnvironmentLightBufferData, sizeof(EnvironmentLightBufferData));
	myContext->Unmap(myEnvironmentLightBuffer, 0);

	myContext->PSSetConstantBuffers(1, 1, &myEnvironmentLightBuffer);
	myContext->PSSetShaderResources(0, 1, myEnvironmentLight->GetCubeMapConstPtr());

	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->IASetInputLayout(nullptr);
	myContext->IAGetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	myContext->VSSetShader(myFullscreenShader, nullptr, 0);

	myContext->PSSetShader(myEnvironmentLightShader, nullptr, 0);
	myContext->Draw(3, 0);
}

void CDeferredRendering::GenerateGBuffer(const std::vector<CModelInstance*>& aModelList)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE bufferdata;
	myFrameBufferData.myToCamera = CU::Matrix4x4f::GetFastInverse(myRenderCamera->GetTransform());
	myFrameBufferData.myToProjection = myRenderCamera->GetProjection();

	myFrameBufferData.myCameraPosition = {
		myRenderCamera->GetPosition().x,
		myRenderCamera->GetPosition().y,
		myRenderCamera->GetPosition().z,
		1.0f
	};

	ZeroMemory(&bufferdata, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = myContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferdata);
	if (FAILED(result))
	{
		assert(false && "Could not define Map from framebuffer.");
	}

	memcpy(bufferdata.pData, &myFrameBufferData, sizeof(FrameBufferData));
	myContext->Unmap(myFrameBuffer, 0);
	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetShaderResources(0, 1, myEnvironmentLight->GetCubeMapConstPtr());

	int modelCount = 0;
	for (CModelInstance* instance : aModelList)
	{
		auto& models = instance->GetModelVector();

		for (size_t i = 0; i < models.size(); i++)
		{
			CModel::SModelData modelData = models[i]->GetModelData();

			myObjectBufferData.myToWorld = instance->GetTransform();
			

			ZeroMemory(&bufferdata, sizeof(D3D11_MAPPED_SUBRESOURCE));
			result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferdata);
			if (FAILED(result))
			{
				assert(false && "Could not define Map from objectbuffer.");
			}
			memcpy(bufferdata.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			myContext->Unmap(myObjectBuffer, 0);

			myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(modelData.myMesh.myPrimitiveTopology));
			myContext->IASetInputLayout(modelData.myMesh.myInputLayout);
			myContext->IASetVertexBuffers(0, 1, &modelData.myMesh.myVertexBuffer, &modelData.myMesh.myStride, &modelData.myMesh.myOffset);
			myContext->IASetIndexBuffer(modelData.myMesh.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);

			myContext->VSSetShader(modelData.myMesh.myVertexShader, nullptr, 0);

			myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
			myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
			myContext->PSSetShader(modelData.myMesh.myPixelShader, nullptr, 0);

			myContext->DrawIndexed(modelData.myMesh.myNumIndices, 0, 0);
		}
		modelCount++;
	}
}
