#include "pch.h"
#include "FullscreenTextureFactory.h"
#include "DirectX11Framework.h"

#include <d3d11.h>

FullscreenTexture FullscreenTextureFactory::CreateTexture(CU::Vector2ui aSize, DXGI_FORMAT aFormat)
{
    HRESULT result;
    D3D11_TEXTURE2D_DESC desc = { 0 };
    desc.Width = aSize.x;
    desc.Height = aSize.y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = aFormat;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    ID3D11Texture2D* texture;
    result = CEngine::GetFramework()->GetDevice()->CreateTexture2D(&desc, nullptr, &texture);
    assert(SUCCEEDED(result));

    FullscreenTexture textureResult = CreateTexture(texture);

    ID3D11ShaderResourceView* SRV;
    result = CEngine::GetFramework()->GetDevice()->CreateShaderResourceView(texture, nullptr, &SRV);
    assert(SUCCEEDED(result));

    textureResult.mySRV = SRV;
    return textureResult;
}

FullscreenTexture FullscreenTextureFactory::CreateTexture(ID3D11Texture2D* aTextureTemplate)
{
    HRESULT result;

    ID3D11RenderTargetView* RTV;
    result = CEngine::GetFramework()->GetDevice()->CreateRenderTargetView(
        aTextureTemplate,
        nullptr,
        &RTV
    );
    assert(SUCCEEDED(result));

    D3D11_VIEWPORT* viewport = nullptr;
    if (aTextureTemplate)
    {
        D3D11_TEXTURE2D_DESC desc;
        aTextureTemplate->GetDesc(&desc);
        viewport = new D3D11_VIEWPORT(
            {
                0,
                0,
                static_cast<float>(desc.Width),
                static_cast<float>(desc.Height),
                0,
                1
            }
        );
    }

    FullscreenTexture textureResult;
    textureResult.myTexture = aTextureTemplate;
    textureResult.myRenderTarget = RTV;
    textureResult.myViewport = viewport;
    return textureResult;
}

FullscreenTexture FullscreenTextureFactory::CreateDepth(CU::Vector2ui aSize, DXGI_FORMAT aFormat)
{
    return FullscreenTexture();
}
