#pragma once

struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct D3D11_VIEWPORT;

class CFullscreenTexture;

class GBuffer
{
public:
	GBuffer() = default;
	~GBuffer() = default;

	enum GBufferTexture
	{
		POSITION,
		ALBEDO,
		NORMAL,
		VERTEXNORMAL,
		MATERIAL,
		AMBIENTOCCLUSION,
		DEPTH,
		COUNT
	};

	void ClearTextures();
	void SetAsActiveTarget(CFullscreenTexture* aDepth = nullptr);
	void SetAsResourceOnSlot(GBufferTexture aTexture, unsigned int aSlot);
	void SetAllAsResources();

private:
	friend class CFullscreenTextureFactory;

	std::array<ID3D11Texture2D*, GBufferTexture::COUNT> myTexture;
	std::array<ID3D11RenderTargetView*, GBufferTexture::COUNT> myRTVs;
	std::array<ID3D11ShaderResourceView*, GBufferTexture::COUNT> mySRVs;
	D3D11_VIEWPORT* myViewport;

};

