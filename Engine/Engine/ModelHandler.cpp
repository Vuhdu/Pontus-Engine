#include "pch.h"
#include "ModelHandler.h"

#include "Model.h"
#include "ModelInstance.h"

#include "MainSingleton.h"
#include "ModelFactory.h"
#include "JsonParser.h"

#include "DirectX11Framework.h"
#include "Inc/DDSTextureLoader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <fstream>
#include <sstream>

CModelHandler::~CModelHandler()
{
	if (myModelVector.size() > 0)
	{
		for (auto& model : myModelVector)
		{
			for (unsigned int modelIndex = 0; modelIndex < model.second.size(); modelIndex++)
			{
				SAFE_DELETE(model.second[modelIndex]);
			}
		}
	}
}

void CModelHandler::Init(ID3D11Device* aDevice)
{
	myDevice = aDevice;

    nlohmann::json models = JsonParser::GetInstance().GetDataFromIDString("Models");

	const int size = static_cast<int>(models["Models"].size());
	for (int modelIndex = 0; modelIndex < size; modelIndex++)
	{
		auto& m = models["Models"][modelIndex];

		std::string modelstringID = m["ModelStringID"];

		std::string modelPath = m["ModelPath"];

		std::string texturePath = m["TexturePath"];
		std::wstring wTexturePath(texturePath.begin(), texturePath.end());

		std::string normalMapPath = m["NormalMapPath"];
		std::wstring wNormalMapPath(normalMapPath.begin(), normalMapPath.end());

		std::string materialMapPath = m["MaterialMapPath"];
		std::wstring wMaterialMapPath(materialMapPath.begin(), materialMapPath.end());

		myModelVector[modelstringID] = LoadModels(modelPath, wTexturePath, wNormalMapPath, wMaterialMapPath);
	}
}

std::vector<CModel*> CModelHandler::GetModelVector(const std::string aModelStringID) const
{
	return myModelVector.at(aModelStringID);
}

std::vector<CModel*> CModelHandler::LoadModels(const std::string& aModelPath, const std::wstring& anAlbedoPath, const std::wstring& aNormalPath, const std::wstring& aMaterialPath)
{
	std::vector<CModel*> modelVector;

	HRESULT result;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(aModelPath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ConvertToLeftHanded);

	aiMesh** meshes = scene->mMeshes;

	const int numberOfMeshes = scene->mNumMeshes;

	struct Vertex
	{
		float x, y, z, w;			// positions
		float r, g, b, a;			// colors
		float u, v;					// uv
		float nx, ny, nz, nw;		// normals
		float tx, ty, tz, tw;		// tangent
		float bx, by, bz, bw;		// binormal
	};

	for (int meshIndex = 0; meshIndex < numberOfMeshes; meshIndex++)
	{
		auto mesh = meshes[meshIndex];

		const unsigned int numberOfVertices = mesh->mNumVertices;
		const unsigned int numberOfFaces = mesh->mNumFaces;
		const unsigned int numberOfIndices = numberOfFaces * 3;

		std::vector<Vertex> vertices;
		vertices.reserve(numberOfVertices);
		vertices.resize(numberOfVertices);

		std::vector<unsigned int> indices;
		indices.reserve(numberOfIndices);
		indices.resize(numberOfIndices);

		auto positions = mesh->mVertices;
		auto colors = mesh->mColors;
		auto UV = mesh->mTextureCoords;
		auto faces = mesh->mFaces;
		auto normals = mesh->mNormals;
		auto tangents = mesh->mTangents;
		auto bitangents = mesh->mBitangents;

		for (unsigned int vertexIndex = 0; vertexIndex < numberOfVertices; vertexIndex++)
		{
			// Set vertex positions
			vertices[vertexIndex].x = positions[vertexIndex].x;
			vertices[vertexIndex].y = positions[vertexIndex].y;
			vertices[vertexIndex].z = positions[vertexIndex].z;
			vertices[vertexIndex].w = 1.0f;

			if (mesh->GetNumColorChannels() > 0)
			{
				vertices[vertexIndex].r = colors[0][vertexIndex].r;
				vertices[vertexIndex].g = colors[0][vertexIndex].g;
				vertices[vertexIndex].b = colors[0][vertexIndex].b;
				vertices[vertexIndex].a = colors[0][vertexIndex].a;
			}
			else 
			{
				vertices[vertexIndex].r = 1.0f;
				vertices[vertexIndex].g = 1.0f;
				vertices[vertexIndex].b = 1.0f;
				vertices[vertexIndex].a = 1.0f;
			}

			vertices[vertexIndex].u = UV[0][vertexIndex].x;
			vertices[vertexIndex].v = UV[0][vertexIndex].y;

			vertices[vertexIndex].nx = normals[vertexIndex].x;
			vertices[vertexIndex].ny = normals[vertexIndex].y;
			vertices[vertexIndex].nz = normals[vertexIndex].z;
			vertices[vertexIndex].nw = 0.0f;

			vertices[vertexIndex].tx = tangents[vertexIndex].x;
			vertices[vertexIndex].ty = tangents[vertexIndex].y;
			vertices[vertexIndex].tz = tangents[vertexIndex].z;
			vertices[vertexIndex].tw = 0.0f;

			vertices[vertexIndex].bx = bitangents[vertexIndex].x;
			vertices[vertexIndex].by = bitangents[vertexIndex].y;
			vertices[vertexIndex].bz = bitangents[vertexIndex].z;
			vertices[vertexIndex].bw = 0.0f;
		}

		int faceIndex = 0;
		for (unsigned int indecesIndex = 0; indecesIndex < numberOfIndices;)
		{
			for (int index = 0; index < 3; index++)
			{
				indices[indecesIndex] = faces[faceIndex].mIndices[index];
				indecesIndex++;
			}
			faceIndex++;
		}

		// Start Vertex
		D3D11_BUFFER_DESC vertexBufferDescription = { 0 };
		vertexBufferDescription.ByteWidth = numberOfVertices * sizeof(vertices[0]);
		vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = { 0 };
		vertexSubresourceData.pSysMem = vertices.data();

		ID3D11Buffer* vertexBuffer;
		result = myDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer);
		if (FAILED(result))
		{
			assert(false && "No vertex buffer could be applied to the model.");
		}

		D3D11_BUFFER_DESC indexBufferDescription = { 0 };
		indexBufferDescription.ByteWidth = numberOfIndices * sizeof(indices[0]);
		indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubresourceData = { 0 };
		indexSubresourceData.pSysMem = indices.data();

		ID3D11Buffer* indexBuffer;
		result = myDevice->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer);
		if (FAILED(result))
		{
			assert(false && "No index buffer could be applied to the model.");
		}
		// End Vertex

		// Start Shader
		std::ifstream vsFile;
		vsFile.open("Shaders/VertexShader.cso", std::ios::binary);
		std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		ID3D11VertexShader* vertexShader;
		result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);

		if (FAILED(result))
		{
			assert(false && "No vertex shader could be applied to the model.");
		}
		vsFile.close();

		std::ifstream psFile;
		psFile.open("Shaders/PixelShader.cso", std::ios::binary);
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
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		ID3D11InputLayout* inputLayout;
		result = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);

		if (FAILED(result))
		{
			assert(false && "No shader layout could be applied to the model.");
		}
		// End Layout

		// Start Textures
		std::wstring albedoPath = anAlbedoPath;
		ID3D11ShaderResourceView* albedoShaderResourceView;
		result = DirectX::CreateDDSTextureFromFile(myDevice, albedoPath.c_str(), nullptr, &albedoShaderResourceView);
		if (FAILED(result))
		{
			const std::string s(anAlbedoPath.begin(), anAlbedoPath.end());
			ERROR_PRINT("Albedo Path \"%s\" could not be found for model.", s.c_str());
		}
		// End Textures

		// Start Normals
		std::wstring normalPath = aNormalPath;
		ID3D11ShaderResourceView* normalShaderResourceView;
		result = DirectX::CreateDDSTextureFromFile(myDevice, normalPath.c_str(), nullptr, &normalShaderResourceView);
		if (FAILED(result))
		{
			const std::string s(aNormalPath.begin(), aNormalPath.end());
			ERROR_PRINT("Normal Path \"%s\" could not be found for model.", s.c_str());
		}
		// End Normals

		// Start Materials
		std::wstring materialPath = aMaterialPath;
		ID3D11ShaderResourceView* materialShaderResourceView;
		result = DirectX::CreateDDSTextureFromFile(myDevice, materialPath.c_str(), nullptr, &materialShaderResourceView);
		if (FAILED(result))
		{
			const std::string s(aMaterialPath.begin(), aMaterialPath.end());
			ERROR_PRINT("Material Path \"%s\" could not be found for model.", s.c_str());
		}
		// End Materials

		CModel* model = new CModel();
		if (!model)
		{
			assert(false && "No memory could be allocated for CModel.");
		}

		CModel::SModelData modelData;
		modelData.myNumberOfVertecies = numberOfVertices;
		modelData.myNumberOfIndices = numberOfIndices;
		modelData.myStride = sizeof(Vertex);
		modelData.myOffset = 0;
		modelData.myVertexBuffer = vertexBuffer;
		modelData.myIndexBuffer = indexBuffer;
		modelData.myVertexShader = vertexShader;
		modelData.myPixelShader = pixelShader;
		modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		modelData.myInputLayout = inputLayout;
		modelData.myTexture[0] = albedoShaderResourceView;
		modelData.myTexture[1] = normalShaderResourceView;
		modelData.myTexture[2] = materialShaderResourceView;
		model->Init(modelData);

		modelVector.push_back(model);
	}
	return modelVector;
}
