#include "pch.h"
#include "FullscreenRenderer.h"
#include "DirectX11Framework.h"

#include <fstream>
#include <d3d11.h>

bool CFullscreenRenderer::Init()
{
    HRESULT result;

    std::ifstream vsFile;
    vsFile.open("Shaders/FullscreenVS.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    result = CEngine::GetFramework()->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &myVertexShader);
    vsFile.close();

    if (FAILED(result))
    {
        return false;
    }
    
    std::array<std::string, Shader::COUNT> shaderPaths;
    shaderPaths[Shader::COPY] = "Shaders/FullscreenPS-Copy.cso";
    shaderPaths[Shader::LUMINANCE] = "Shaders/FullscreenPS-Luminance.cso";
    shaderPaths[Shader::GAUSSIANHORIZONTAL] = "Shaders/FullscreenPS-GaussianH.cso";
    shaderPaths[Shader::GAUSSIANVERTICAL] = "Shaders/FullscreenPS-GaussianV.cso";
    shaderPaths[Shader::BLOOM] = "Shaders/FullscreenPS-Bloom.cso";
        
    for (unsigned int i = 0; i < Shader::COUNT; i++)
    {
        std::ifstream psFile;
        psFile.open(shaderPaths[i], std::ios::binary);
        std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
        result = CEngine::GetFramework()->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShaders[i]);
        psFile.close();
        if (FAILED(result))
        {
            return false;
        }
    }
    return true;
}

void CFullscreenRenderer::Render(Shader aMode)
{
    auto context = CEngine::GetFramework()->GetContext();
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IAGetInputLayout(nullptr);
    context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

    context->VSSetShader(myVertexShader, nullptr, 0);
    context->PSSetShader(myPixelShaders[aMode], nullptr, 0);
    context->Draw(3, 0);
}
