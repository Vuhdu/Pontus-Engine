#include "pch.h"
#include "ModelFactory.h"
#include "DirectX11Framework.h"
#include "Inc/DDSTextureLoader.h"

#include <fstream>

#include "Model.h"
#include "ModelInstance.h"

#include "Scene.h"
#include "JsonParser.h"


CModelFactory::CModelFactory()
{
	
}

CModelFactory::~CModelFactory()
{
}

void CModelFactory::Init(ID3D11Device* aDevice)
{
	myDevice = aDevice;
	myModelHandler.Init(myDevice);
}

std::vector<CModel*> CModelFactory::GetCube()
{
	HRESULT result;

	SVertex vertices[24] =
	{
		//Back
		{ 1, -1, 1, 1, 1, 0, 1, 1, 0, 0 },
		{ -1, -1, 1, 1, 1, 1, 0, 1, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		{ -1, 1, 1, 1, 0, 1, 1, 1, 1, 1 },

		//Right
		{ 1, -1, -1, 1, 1, 0, 0, 1, 0, 0 },
		{ 1, -1, 1, 1, 0, 0, 1, 1, 1, 0 },
		{ 1, 1, -1, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 1, 1, 1, 0, 0, 0, 1, 1, 1 },

		//Top
		{ -1, 1, -1, 1, 1, 1, 1, 1, 0, 0 },
		{ 1, 1, -1, 1, 0, 1, 1, 1, 1, 0 },
		{ -1, 1, 1, 1, 1, 0, 1, 1, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },

		//Left
		{ -1, -1, 1, 1, 1, 0, 0, 1, 0, 0 },
		{ -1, -1, -1, 1, 0, 1, 0, 1, 1, 0 },
		{ -1, 1, 1, 1, 0, 0, 1, 1, 0, 1 },
		{ -1, 1, -1, 1, 0, 0, 0, 1, 1, 1 },

		//Bottom
		{ -1, -1, 1, 1, 1, 1, 1, 1, 0, 0 },
		{ 1, -1, 1, 1, 1, 1, 0, 1, 1, 0 },
		{ -1, -1, -1, 1, 1, 0, 1, 1, 0, 1 },
		{ 1, -1, -1, 1, 0, 1, 1, 1, 1, 1 },

		//Front
		{ -1, -1, -1, 1, 1, 0, 0, 1, 0, 0 },
		{ 1, -1, -1, 1, 0, 1, 0, 1, 1, 0 },
		{ -1, 1, -1, 1, 0, 0, 1, 1, 0, 1 },
		{ 1, 1, -1, 1, 0, 0, 0, 1, 1, 1 }
	};

	unsigned int indices[36] =
	{
		0, 2, 3,
		0, 3, 1,
		4, 7, 5,
		4, 6, 7,
		8, 11, 9,
		8, 10, 11,
		12, 15, 13,
		12, 14, 15,
		16, 19, 17,
		16, 18, 19,
		20, 23, 21,
		20, 22, 23
	};

	// Start Vertex
	D3D11_BUFFER_DESC vertexBufferDescription = { 0 };
	vertexBufferDescription.ByteWidth = sizeof(vertices);
	vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = { 0 };
	vertexSubresourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer);
	if (FAILED(result))
	{
		assert(false && "No vertex buffer could be applied to the model.");
	}

	D3D11_BUFFER_DESC indexBufferDescription = { 0 };
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData = { 0 };
	indexSubresourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		assert(false && "No index buffer could be applied to the model.");
	}
	// End Vertex

	// Start Shader
	std::ifstream vsFile;
	vsFile.open("CubeVertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);

	if (FAILED(result))
	{
		assert(false && "No vertex shader could be applied to the model.");
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("CubePixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);

	if (FAILED(result))
	{
		assert(false && "No pixel shader could be applied to the model.");
	}
	psFile.close();
	// End Shader

	// Start Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);

	if (FAILED(result))
	{
		assert(false && "No shader layout could be applied to the model.");
	}
	// End Layout

	// Start Textures
	std::wstring filename = L"Texture.dds";
	ID3D11ShaderResourceView* shaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), nullptr, &shaderResourceView);
	if (FAILED(result))
	{
		assert(false && "No albedo path could be found for model.");
	}
	// End Textures

	CModel* model = new CModel();
	if (!model)
	{
		assert(false && "No memory could be allocated for CModel.");
	}

	CModel::SModelData modelData;
	modelData.myMesh.myNumVertices = sizeof(vertices) / sizeof(SVertex);
	modelData.myMesh.myNumIndices = sizeof(indices) / sizeof(unsigned int);
	modelData.myMesh.myStride = sizeof(SVertex);
	modelData.myMesh.myOffset = 0;
	modelData.myMesh.myVertexBuffer = vertexBuffer;
	modelData.myMesh.myIndexBuffer = indexBuffer;
	modelData.myMesh.myVertexShader = vertexShader;
	modelData.myMesh.myPixelShader = pixelShader;
	modelData.myMesh.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myMesh.myInputLayout = inputLayout;
	modelData.myTexture[0] = shaderResourceView;
	model->Init(modelData);

	std::vector<CModel*> models;
	models.push_back(model);
	return models;
}

CModelInstance* CModelFactory::CreateModel(const char* aModelStringID)
{
	auto modelInstance = new CModelInstance();

	modelInstance->Init(myModelHandler.GetModelVector(aModelStringID));

	CEngine::GetScene()->AddInstance(modelInstance);
	
	return modelInstance;
}

CModelInstance* CModelFactory::CreateModel(const char* aModelStringID, const CU::Vector3f& aPosition)
{
	auto instance = CreateModel(aModelStringID);

	instance->SetPosition(aPosition);
	
	return instance;
}

void CModelFactory::LoadAndCreateModelFromDrive(const char* aModelStringID, const CU::Vector3f& aPosition)
{
	nlohmann::json models = JsonParser::GetInstance().GetDataFromIDString("Models");

	const int size = static_cast<int>(models["Models"].size());
	for (int modelIndex = 0; modelIndex < size; modelIndex++)
	{
		auto& m = models["Models"][modelIndex];

		std::string modelstringID = m["ModelStringID"];

		if (strcmp(modelstringID.c_str(), aModelStringID) == 0)
		{
			std::string modelPath = m["ModelPath"];

			std::string texturePath = m["TexturePath"];
			std::wstring wTexturePath(texturePath.begin(), texturePath.end());

			std::string normalMapPath = m["NormalMapPath"];
			std::wstring wNormalMapPath(normalMapPath.begin(), normalMapPath.end());

			std::string materialMapPath = m["MaterialMapPath"];
			std::wstring wMaterialMapPath(materialMapPath.begin(), materialMapPath.end());

			auto models = myModelHandler.LoadModels(modelPath, wTexturePath, wNormalMapPath, wMaterialMapPath);

			auto instance = new CModelInstance();
			instance->Init(models);
			instance->SetPosition(aPosition);
			CEngine::GetScene()->AddInstance(instance);
		}
	}
}