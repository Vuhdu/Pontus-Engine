#pragma once

class FullscreenTexture
{
public:
	FullscreenTexture() = default;
	~FullscreenTexture() = default;

	void ClearTexture(CU::Vector4f aClearColor = { 0.0f, 0.0f, 0.0f, 0.0f });
	void ClearDepth(float aClearDepthValue = 1.0f, unsigned int aClearStencilValue = 0.0f);
	void SetAsActiveTarget(FullscreenTexture* aDepth = nullptr);
	void SetAsResourceOnSlot(unsigned int aSlot);

private:
	friend class FullscreenTextureFactory;

	union
	{
		struct ID3D11RenderTargetView* myRenderTarget;
		struct ID3D11DepthStencilView* myDepth;
	};

	struct ID3D11Texture2D* myTexture = nullptr;
	struct ID3D11ShaderResourceView* mySRV = nullptr;
	struct D3D11_VIEWPORT* myViewport = nullptr;
};

