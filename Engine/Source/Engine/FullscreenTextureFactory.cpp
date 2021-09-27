#include "pch.h"
#include "FullscreenTextureFactory.h"
#include "DirectX11Framework.h"

#include <d3d11.h>

CFullscreenTexture CFullscreenTextureFactory::CreateTexture(CU::Vector2ui aSize, DXGI_FORMAT aFormat)
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

    CFullscreenTexture textureResult = CreateTexture(texture);

    ID3D11ShaderResourceView* SRV;
    result = CEngine::GetFramework()->GetDevice()->CreateShaderResourceView(texture, nullptr, &SRV);
    assert(SUCCEEDED(result));

    textureResult.mySRV = SRV;
    return textureResult;
}

CFullscreenTexture CFullscreenTextureFactory::CreateTexture(ID3D11Texture2D* aTextureTemplate)
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

    CFullscreenTexture textureResult;
    textureResult.myTexture = aTextureTemplate;
    textureResult.myRenderTarget = RTV;
    textureResult.myViewport = viewport;
    return textureResult;
}

CFullscreenTexture CFullscreenTextureFactory::CreateShadowMap(const CU::Vector2ui& aSize)
{
    HRESULT result;

    D3D11_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = aSize.x;
    textureDesc.Height = aSize.y;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    ID3D11Texture2D* texture;
    result = CEngine::GetFramework()->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture);
    assert(SUCCEEDED(result));

    D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    ID3D11DepthStencilView* depth;
    result = CEngine::GetFramework()->GetDevice()->CreateDepthStencilView(texture, &depthDesc, &depth);
    assert(SUCCEEDED(result));

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
    resourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
    resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceDesc.Texture2D.MipLevels = textureDesc.MipLevels;

    ID3D11ShaderResourceView* shaderResource;
    result = CEngine::GetFramework()->GetDevice()->CreateShaderResourceView(texture, &resourceDesc, &shaderResource);
    assert(SUCCEEDED(result));

    D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT(
        { 
            0.0f, 
            0.0f, 
            static_cast<float>(aSize.x), 
            static_cast<float>(aSize.y), 
            0.0f, 
            1.0f 
        });

    CFullscreenTexture returnDepth;
    returnDepth.myTexture = texture;
    returnDepth.myDepth = depth;
    returnDepth.mySRV = shaderResource;
    returnDepth.myViewport = viewport;
    return returnDepth;
}

CFullscreenTexture CFullscreenTextureFactory::CreateDepth(CU::Vector2ui aSize, DXGI_FORMAT aFormat)
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
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    ID3D11Texture2D* texture;
    result = CEngine::GetFramework()->GetDevice()->CreateTexture2D(
        &desc, 
        nullptr, 
        &texture
    );
    assert(SUCCEEDED(result));

    ID3D11DepthStencilView* DSV;
    result = CEngine::GetFramework()->GetDevice()->CreateDepthStencilView(
        texture,
        nullptr,
        &DSV
    );
    assert(SUCCEEDED(result));

    D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT(
        {
            0,
            0,
            static_cast<float>(aSize.x),
            static_cast<float>(aSize.y),
            0,
            1
        }
    );

    CFullscreenTexture textureResult;
    textureResult.myTexture = texture;
    textureResult.myDepth = DSV;
    textureResult.myViewport = viewport;
    return textureResult;
}

GBuffer CFullscreenTextureFactory::CreateGBuffer(CU::Vector2ui aSize)
{
    HRESULT result;

    std::array<DXGI_FORMAT, GBuffer::COUNT> textureFormats =
    {
        DXGI_FORMAT_R32G32B32A32_FLOAT, // POSITION
        DXGI_FORMAT_R8G8B8A8_UNORM,     // ALBEDO
        DXGI_FORMAT_R16G16B16A16_SNORM, // NORMAL
        DXGI_FORMAT_R16G16B16A16_SNORM, // VERTEXNORMAL
        DXGI_FORMAT_R8G8B8A8_UNORM,     // MATERIAL
        DXGI_FORMAT_R8_UNORM,           // AMBIENTOCCLUSION
        DXGI_FORMAT_R16_UNORM           // DEPTH
    };

    GBuffer returnGBuffer = {};

    D3D11_TEXTURE2D_DESC desc = { 0 };
    desc.Width = aSize.x;
    desc.Height = aSize.y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    for (unsigned int idx = 0; idx < GBuffer::COUNT; idx++)
    {
        auto device = CEngine::GetFramework()->GetDevice();

        desc.Format = textureFormats[idx];
        result = device->CreateTexture2D(&desc, nullptr, &returnGBuffer.myTexture[idx]);
        assert(SUCCEEDED(result));

        result = device->CreateRenderTargetView(
            returnGBuffer.myTexture[idx],
            nullptr,
            &returnGBuffer.myRTVs[idx]
        );
        assert(SUCCEEDED(result));

        result = device->CreateShaderResourceView(
            returnGBuffer.myTexture[idx],
            nullptr,
            &returnGBuffer.mySRVs[idx]
        );
        assert(SUCCEEDED(result));
    }

    returnGBuffer.myViewport = new D3D11_VIEWPORT(
        {
            0,
            0,
            static_cast<float>(desc.Width),
            static_cast<float>(desc.Height),
            0,
            1
        }
    );
    return returnGBuffer;
}
