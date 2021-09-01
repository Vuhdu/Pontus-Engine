#include "pch.h"
#include "Model.h"

CModel::CModel()
{
}

CModel::~CModel()
{
}

void CModel::Init(const SModelData& aModelData)
{
    myModelData = aModelData;
}

CModel::SModelData CModel::GetModelData()
{
    return myModelData;
}
