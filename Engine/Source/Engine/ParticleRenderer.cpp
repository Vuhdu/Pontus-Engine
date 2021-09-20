#include "pch.h"
#include "ParticleRenderer.h"
#include "DirectX11Framework.h"
#include "ParticleEmitterInstance.h"

#include "Camera.h"

#include <fstream>
#include <d3d11.h>

bool CParticleRenderer::Init(CDirectX11Framework* aFramework)
{
	myContext = aFramework->GetContext();

	return true;
}

void CParticleRenderer::SetRenderCamera(CCamera* aCamera)
{
	myRenderCamera = aCamera;
}

void CParticleRenderer::Render(const std::vector<std::shared_ptr<CParticleEmitterInstance>>& someParticleEmitters)
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
		return;
	}

	memcpy(bufferdata.pData, &myFrameBufferData, sizeof(FrameBufferData));
	myContext->Unmap(myFrameBuffer, 0);
	
	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->GSSetConstantBuffers(0, 1, &myFrameBuffer);

    for (const auto& instance : someParticleEmitters)
    {
        const auto& emitter = instance->GetEmitter();
        myObjectBufferData.myToWorld = instance->GetTransform();
        myObjectBufferData.myUVScale = { 1.0f, 1.0f };

        ZeroMemory(&bufferdata, sizeof(D3D11_MAPPED_SUBRESOURCE));
        result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferdata);
        assert(SUCCEEDED(result));
        memcpy(bufferdata.pData, &myObjectBufferData, sizeof(ObjectBufferData));
        myContext->Unmap(myObjectBuffer, 0);

        CParticleEmitter::SParticleData emitterData = emitter->GetData();

        ZeroMemory(&bufferdata, sizeof(D3D11_MAPPED_SUBRESOURCE));
        result = myContext->Map(emitterData.myParticleVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferdata);
        assert(SUCCEEDED(result));
        memcpy(bufferdata.pData, &(instance->GetVertices()[0]), sizeof(CParticleEmitter::SParticleVertex) * instance->GetVertices().size());
        myContext->Unmap(emitterData.myParticleVertexBuffer, 0);

        myContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(emitterData.myPrimitiveTopology));
        myContext->IASetInputLayout(emitterData.myInputLayout);
        myContext->IASetVertexBuffers(0, 1, &emitterData.myParticleVertexBuffer, &emitterData.myStride, &emitterData.myOffset);

        myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
        myContext->VSSetShader(emitterData.myParticleVertexShader, nullptr, 0);

        myContext->GSSetShader(emitterData.myParticleGeometryShader, nullptr, 0);

        myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
        myContext->PSSetShaderResources(0, 1, &emitterData.mySRV);
        myContext->PSSetShader(emitterData.myParticlePixelShader, nullptr, 0);

        myContext->Draw(emitterData.myNumberOfParticles, 0);
    }
}
