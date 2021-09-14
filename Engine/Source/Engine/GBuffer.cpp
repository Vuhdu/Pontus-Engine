#include "pch.h"
#include "GBuffer.h"
#include "FullscreenTexture.h"
#include "DirectX11Framework.h"

#include <d3d11.h>

void GBuffer::ClearTextures()
{
	const auto clearColor = CU::Vector4f{ 0.0f, 0.0f, 0.0f, 1.0f };

	for (int i = 0; i < GBufferTexture::COUNT; i++)
	{
		CEngine::GetFramework()->GetContext()->ClearRenderTargetView(
			myRTVs[static_cast<GBufferTexture>(i)],
			&clearColor.x
		);
	}
}

void GBuffer::SetAsActiveTarget(CFullscreenTexture* aDepth)
{
	auto context = CEngine::GetFramework()->GetContext();

	context->OMSetRenderTargets(
		GBufferTexture::COUNT,
		&myRTVs[0],
		(aDepth) ? aDepth->GetDepthStencil() : nullptr
	);

	context->RSSetViewports(1, myViewport);
}

void GBuffer::SetAsResourceOnSlot(GBufferTexture aTexture, unsigned int aSlot)
{
	CEngine::GetFramework()->GetContext()->PSSetShaderResources(aSlot, 1, &mySRVs[aTexture]);
}

void GBuffer::SetAllAsResources()
{
	CEngine::GetFramework()->GetContext()->PSSetShaderResources(1, GBufferTexture::COUNT, &mySRVs[0]);
}
