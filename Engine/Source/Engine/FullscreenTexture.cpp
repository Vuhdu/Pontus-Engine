#include "pch.h"
#include "FullscreenTexture.h"
#include "DirectX11Framework.h"

#include <d3d11.h>

void FullscreenTexture::ClearTexture(CU::Vector4f aClearColor)
{
	CEngine::GetFramework()->GetContext()->ClearRenderTargetView(
		myRenderTarget, 
		&aClearColor.x
	);
}

void FullscreenTexture::ClearDepth(float aClearDepthValue, unsigned int aClearStencilValue)
{
	CEngine::GetFramework()->GetContext()->ClearDepthStencilView(
		myDepth, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		aClearDepthValue, 
		aClearStencilValue
	);
}

void FullscreenTexture::SetAsActiveTarget(FullscreenTexture* aDepth)
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

void FullscreenTexture::SetAsResourceOnSlot(unsigned int aSlot)
{
	CEngine::GetFramework()->GetContext()->PSSetShaderResources(aSlot, 1, &mySRV);
}
