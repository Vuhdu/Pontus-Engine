#pragma once

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class CFullscreenTexture
{
public:
	CFullscreenTexture() = default;
	~CFullscreenTexture() = default;

	void ClearTexture(CU::Vector4f aClearColor = { 0.0f, 0.0f, 0.0f, 1.0f });
	void ClearDepth(float aClearDepthValue = 1.0f, UINT8 aClearStencilValue = 0);
	void SetAsActiveTarget(CFullscreenTexture* aDepth = nullptr);
	void SetAsResourceOnSlot(unsigned int aSlot);

	ID3D11RenderTargetView* GetRenderTarget() { return myRenderTarget; }
	ID3D11DepthStencilView* GetDepthStencil() { return myDepth; }

private:
	friend class CFullscreenTextureFactory;

	union
	{
		ID3D11RenderTargetView* myRenderTarget;
		ID3D11DepthStencilView* myDepth;
	};

	struct ID3D11Texture2D* myTexture = nullptr;
	struct ID3D11ShaderResourceView* mySRV = nullptr;
	struct D3D11_VIEWPORT* myViewport = nullptr;
};

