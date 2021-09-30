#include "pch.h"
#include "ModelInstance.h"
#include "Model.h"

#include <sstream>

CModelInstance::CModelInstance()
{
}

CModelInstance::~CModelInstance()
{

}

void CModelInstance::Init(const std::vector<CModel*>& aModelVector)
{
    myModelVector = aModelVector;
}

void CModelInstance::SetTransform(const CU::Transform& aTransform)
{
	myTransform = aTransform;
}

void CModelInstance::SetScale(const CU::Vector3f aScale)
{
	myTransform.SetScale(aScale);
}

void CModelInstance::SetRotation(const CU::Vector3f aRotation)
{
	myTransform.SetRotation(aRotation);
}

void CModelInstance::SetPosition(const CU::Vector3f aPosition)
{
	myTransform.SetPosition(aPosition);
}

void CModelInstance::Move(const CU::Vector3f aMovement)
{
	/*
	CU::Vector3f position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};
	
	position.x += aMovement.x;
	position.y += aMovement.y;
	position.z += aMovement.z;
	
	SetPosition(position);
	*/
}

void CModelInstance::Rotate(const CU::Vector3f aRotation)
{
	/*
	const CU::Vector3f position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	myTransform *= CU::Matrix4x4f::CreateRotationAroundX(aRotation.x);
	myTransform *= CU::Matrix4x4f::CreateRotationAroundY(aRotation.y);
	myTransform *= CU::Matrix4x4f::CreateRotationAroundZ(aRotation.z);

	SetPosition(position);
	*/
}
