#include "pch.h"
#include "ForwardRenderer.h"
#include "DirectX11Framework.h"
#include "Camera.h"
#include "Model.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "SpotLight.h"

CForwardRenderer::CForwardRenderer()
{
	myContext = nullptr;
	myFrameBuffer = nullptr;
	myObjectBuffer = nullptr;
}

CForwardRenderer::~CForwardRenderer()
{
}

bool CForwardRenderer::Init(CDirectX11Framework* aFramework)
{
	if (!aFramework)
	{
		return false;
	}

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

void CForwardRenderer::Render(CEnvironmentLight* anEnvironmentLight, const std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>>& somePointLights, const std::vector<std::pair<unsigned int, std::array<CSpotLight*, 8>>>& someSpotLights, CCamera* aCamera, std::vector<CModelInstance*>& aModelList)
{
	HRESULT result;
	
	D3D11_MAPPED_SUBRESOURCE bufferdata;
	myFrameBufferData.myToCamera = CU::Matrix4x4f::GetFastInverse(aCamera->GetTransform());
	myFrameBufferData.myToProjection = aCamera->GetProjection();

	myFrameBufferData.myCameraPosition = {
		aCamera->GetPosition().x,
		aCamera->GetPosition().y,
		aCamera->GetPosition().z,
		1.0f
	};

	myFrameBufferData.myDirectionalLightDirection = anEnvironmentLight->GetDirection();
	myFrameBufferData.myDirectionalLightColor = anEnvironmentLight->GetColor();

	ID3D11Resource* cubeResource = nullptr;
	anEnvironmentLight->GetCubeMap()->GetResource(&cubeResource);
	ID3D11Texture2D* cubeTexture = reinterpret_cast<ID3D11Texture2D*>(cubeResource);
	D3D11_TEXTURE2D_DESC cubeDescription = {};
	cubeTexture->GetDesc(&cubeDescription);
	myFrameBufferData.myEnvironmentLightMipCount = cubeDescription.MipLevels;
	cubeResource->Release();

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
	myContext->PSSetShaderResources(0, 1, anEnvironmentLight->GetCubeMapConstPtr());

	int modelCount = 0;
	for (CModelInstance* instance : aModelList)
	{
		auto& models = instance->GetModelVector();

		for (size_t i = 0; i < models.size(); i++)
		{
			CModel::SModelData modelData = models[i]->GetModelData();

			myObjectBufferData.myToWorld = instance->GetTransform();
			int pointLightSize = static_cast<unsigned int>(somePointLights[modelCount].first);
			int spotLightSize = static_cast<unsigned int>(someSpotLights[modelCount].first);
			
			myObjectBufferData.myNumPointLights = pointLightSize;
			myObjectBufferData.myNumSpotLights = spotLightSize;

			for (int j = 0; j < pointLightSize; j++)
			{
				auto pos = somePointLights[modelCount].second[j]->GetPosition();
				auto color = somePointLights[modelCount].second[j]->GetColor();
				auto intensity = somePointLights[modelCount].second[j]->GetIntensity();

				myObjectBufferData.myPointLights[j].myPosition = {
					pos.x,
					pos.y,
					pos.z,
					1.0f
				};
				myObjectBufferData.myPointLights[j].myColorAndIntensity = {
					color.x,
					color.y,
					color.z,
					intensity
				};
				myObjectBufferData.myPointLights[j].myRange = somePointLights[modelCount].second[j]->GetRange();
			}

			for (int j = 0; j < spotLightSize; j++)
			{
				auto pos = someSpotLights[modelCount].second[j]->GetPosition();
				auto dir = someSpotLights[modelCount].second[j]->GetDirection();
				auto color = someSpotLights[modelCount].second[j]->GetColor();
				auto intensity = someSpotLights[modelCount].second[j]->GetIntensity();

				myObjectBufferData.mySpotLights[j].myPosition = {
					pos.x,
					pos.y,
					pos.z,
					1.0f
				};

				myObjectBufferData.mySpotLights[j].myDirection = {
					dir.x,
					dir.y,
					dir.z,
					0.0f
				};

				myObjectBufferData.mySpotLights[j].myColorAndIntensity = {
					color.x,
					color.y,
					color.z,
					intensity
				};
				myObjectBufferData.mySpotLights[j].myRange = someSpotLights[modelCount].second[j]->GetRange();
				myObjectBufferData.mySpotLights[j].myInnerAngle = someSpotLights[modelCount].second[j]->GetInnerRadius();
				myObjectBufferData.mySpotLights[j].myOuterAngle = someSpotLights[modelCount].second[j]->GetOuterRadius();
			}

			ZeroMemory(&bufferdata, sizeof(D3D11_MAPPED_SUBRESOURCE));
			result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferdata);
			if (FAILED(result))
			{
				assert(false && "Could not define Map from objectbuffer.");
			}
			memcpy(bufferdata.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			myContext->Unmap(myObjectBuffer, 0);

			myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
			myContext->IASetInputLayout(modelData.myInputLayout);
			myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
			myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);

			myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);

			myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
			myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
			myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

			myContext->DrawIndexed(modelData.myNumberOfIndices, 0, 0);
		}
		modelCount++;
	}
}
