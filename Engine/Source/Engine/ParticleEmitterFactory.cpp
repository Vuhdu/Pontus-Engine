#include "pch.h"
#include "ParticleEmitterFactory.h"
#include "ParticleEmitter.h"
#include "ParticleEmitterInstance.h"

#include "Scene.h"

#include "JsonParser.h"

#include "DirectX11Framework.h"
#include "Inc/DDSTextureLoader.h"

#include <fstream>
#include <d3d11.h>

void CParticleEmitterFactory::Init(ID3D11Device* aDevice)
{
    myDevice = aDevice;
    std::shared_ptr<CParticleEmitter> emitter;

    nlohmann::json particles = JsonParser::GetInstance().GetDataFromIDString("Particles");

    const int size = static_cast<int>(particles["Particles"].size());
    for (int particleIndex = 0; particleIndex < size; particleIndex++)
    {
        auto& p = particles["Particles"][particleIndex];

        std::string particleNameID = p["ParticleNameID"];

        std::string texturePath = p["TexturePath"];
        std::wstring wTexturePath(texturePath.begin(), texturePath.end());

        float spawnRate = p["SpawnRate"];
        float lifetime = p["Lifetime"];

        if (myParticleEmitters[particleNameID] = LoadParticleEmitter(wTexturePath, std::ceil(lifetime / spawnRate)))
        {
            float spawnAngle = p["SpawnAngle"];
            float speed = p["Speed"];
            float startSize = p["StartSize"];
            float endSize = p["EndSize"];
            float emissiveStrength = p["EmissiveStrength"];

            CU::Vector4f startColor = {
                p["StartColor"]["r"],
                p["StartColor"]["g"],
                p["StartColor"]["b"],
                p["StartColor"]["a"]
            };

            CU::Vector4f endColor = {
                p["EndColor"]["r"],
                p["EndColor"]["g"],
                p["EndColor"]["b"],
                p["EndColor"]["a"]
            };

            CParticleEmitter::SParticleSettings settings;
            settings.mySpawnRate = spawnRate;
            settings.mySpawnAngle = spawnAngle;
            settings.myParticleLifetime = lifetime;
            settings.myParticleSpeed = speed;
            settings.myParticleStartSize = startSize;
            settings.myParticleEndSize = endSize;
            settings.myParticleEmissiveStrength = emissiveStrength;
            settings.myParticleStartColor = startColor;
            settings.myParticleEndColor = endColor;

            myParticleEmitters[particleNameID]->SetSettings(settings);
        }
        else
        {
            assert(false && "Failed to load particle with NameID: \"%s\"", particleNameID.c_str());
        }
    }
}

CParticleEmitterInstance* CParticleEmitterFactory::GetParticleEmitter(const std::string& aParticleNameID)
{
    CParticleEmitter* emitter = nullptr;
    
    if (!GetParticleEmitterInternal(aParticleNameID, emitter))
    {
        assert(false && "Could not create particle emitter from file path \"%s\"", someFilePath.c_str());
        return nullptr;
    }
    
    CParticleEmitterInstance* instance = new CParticleEmitterInstance();
    instance->Init(emitter);

    CEngine::GetScene()->AddInstance(instance);

    return instance;
}

bool CParticleEmitterFactory::GetParticleEmitterInternal(const std::string& aParticleNameID, CParticleEmitter*& anOutEmitter)
{
    if (myParticleEmitters.find(aParticleNameID) != myParticleEmitters.end())
    {
        anOutEmitter = myParticleEmitters[aParticleNameID];
        return true;
    }
    return false;
}

CParticleEmitter* CParticleEmitterFactory::LoadParticleEmitter(const std::wstring& aTexturePath, const int aMaxAmount)
{
    HRESULT result;

    ID3D11Buffer* vertexBuffer;
    D3D11_BUFFER_DESC bufferDescription = { 0 };
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDescription.ByteWidth = aMaxAmount * sizeof(CParticleEmitter::SParticleVertex);

    result = myDevice->CreateBuffer(&bufferDescription, nullptr, &vertexBuffer);
    if (FAILED(result))
    {
        assert(false && "No vertex buffer could be applied to the emitter.");
    }

    std::ifstream vsFile;
    vsFile.open("Shaders/ParticleVS.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    ID3D11VertexShader* vertexShader;
    result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
    if (FAILED(result))
    {
        assert(false && "Failed to create Vertex Shader.");
    }
    vsFile.close();


    std::ifstream gsFile;
    gsFile.open("Shaders/ParticleGS.cso", std::ios::binary);
    std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
    ID3D11GeometryShader* geometryShader;
    result = myDevice->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, &geometryShader);
    if (FAILED(result))
    {
        assert(false && "Failed to create Geometry Shader.");
    }
    gsFile.close();


    std::ifstream psFile;
    psFile.open("Shaders/ParticlePS.cso", std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    ID3D11PixelShader* pixelShader;
    result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
    if (FAILED(result))
    {
        assert(false && "Failed to create Pixel Shader.");
    }
    psFile.close();

    // Start Layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "VELOCITY", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "DISTANCE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "EMISSIVE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    ID3D11InputLayout* inputLayout;
    result = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
    if (FAILED(result))
    {
        assert(false && "Failed to map Input Layout.");
    }

    std::wstring texturePath = aTexturePath;
    ID3D11ShaderResourceView* textureShaderResourceView;
    result = DirectX::CreateDDSTextureFromFile(myDevice, texturePath.c_str(), nullptr, &textureShaderResourceView);
    if (FAILED(result))
    {
        const std::string s(texturePath.begin(), texturePath.end());
        ERROR_PRINT("Texture Path \"%s\" could not be found for particle.", s.c_str());
    }

    CParticleEmitter::SParticleData data;
    data.myNumberOfParticles = aMaxAmount;
    data.myStride = sizeof(CParticleEmitter::SParticleVertex);
    data.myOffset = 0;
    data.myParticleVertexBuffer = vertexBuffer;
    data.myParticleVertexShader = vertexShader;
    data.myParticleGeometryShader = geometryShader;
    data.myParticlePixelShader = pixelShader;
    data.myInputLayout = inputLayout;
    data.myPrimitiveTopology = static_cast<unsigned int>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    data.mySRV = textureShaderResourceView;

    CParticleEmitter* emitter = new CParticleEmitter();
    emitter->SetData(data);
    return emitter;
}
