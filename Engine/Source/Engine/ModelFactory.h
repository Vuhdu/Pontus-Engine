#pragma once
#include "ModelHandler.h"

struct ID3D11Device;

class CModel;
class CModelInstance;

class CModelFactory
{
public:
	CModelFactory();
	~CModelFactory();

	void Init(ID3D11Device* aDevice);

	std::vector<CModel*> GetCube();

	CModelInstance* CreateModel(const char* aModelStringID);
	CModelInstance* CreateModel(const char* aModelStringID, const CU::Vector3f& aPosition);
	
	void LoadAndCreateModelFromDrive(const char* aModelStringID, CModelInstance* aModelInstance);

private:
	CModelHandler myModelHandler;
	ID3D11Device* myDevice = nullptr;

};

