#include "pch.h"
#include "FullscreenTexture.h"
#include "DirectX11Framework.h"

#include <d3d11.h>

void CFullscreenTexture::ClearTexture(CU::Vector4f aClearColor)
{
	CEngine::GetFramework()->GetContext()->ClearRenderTargetView(
		myRenderTarget, 
		&aClearColor.x
	);
}

void CFullscreenTexture::ClearDepth(float aClearDepthValue, unsigned char aClearStencilValue)
{
	CEngine::GetFramework()->GetContext()->ClearDepthStencilView(
		myDepth, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		aClearDepthValue, 
		aClearStencilValue
	);
}

void CFullscreenTexture::SetAsActiveDepth()
{
	CEngine::GetFramework()->GetContext()->OMSetRenderTargets(0, nullptr, myDepth);
	CEngine::GetFramework()->GetContext()->RSSetViewports(1, myViewport);
}

void CFullscreenTexture::SetAsActiveTarget(CFullscreenTexture* aDepth)
{
	if (aDepth)
	{
		CEngine::GetFramework()->GetContext()->OMSetRenderTargets(
			1,
			&myRenderTarget,
			aDepth->myDepth
		);
	}
	else 
	{
		CEngine::GetFramework()->GetContext()->OMSetRenderTargets(
			1,
			&myRenderTarget,
			nullptr
		);
	}
	CEngine::GetFramework()->GetContext()->RSSetViewports(1, myViewport);
}

void CFullscreenTexture::SetAsResourceOnSlot(unsigned int aSlot)
{
	CEngine::GetFramework()->GetContext()->PSSetShaderResources(aSlot, 1, &mySRV);
}
