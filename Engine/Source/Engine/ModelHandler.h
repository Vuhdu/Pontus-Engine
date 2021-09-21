#pragma once
#include <unordered_map>

class CModel;
class CModelInstance;
struct ID3D11Device;

class CModelHandler
{
public:
	CModelHandler() = default;
	~CModelHandler();

	void Init(ID3D11Device* aDevice);

	std::vector<CModel*>	GetModelVector(const std::string aModelStringID) const;
	std::vector<CModel*>	LoadModels(const std::string& aModelPath, const std::wstring& anAlbedoPath, const std::wstring& aNormalPath, const std::wstring& aMaterialPath);

private:
	ID3D11Device* myDevice = nullptr;

	std::unordered_map<std::string, std::vector<CModel*>> myModelVector;

};