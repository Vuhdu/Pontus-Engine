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

void CModelInstance::SetTransform(const CU::Vector3f aPosition, CU::Vector3f aRotation)
{
    SetPosition(aPosition);
    SetRotation(aRotation);
}

void CModelInstance::SetRotation(const CU::Vector3f aRotation)
{
	CU::Matrix4x4f translation;
	translation(4, 1) = myTransform(4, 1);
	translation(4, 2) = myTransform(4, 2);
	translation(4, 3) = myTransform(4, 3);

	const CU::Matrix4x4f newMatrix =
		CU::Matrix4x4f::CreateRotationAroundZ(aRotation.z) *
		CU::Matrix4x4f::CreateRotationAroundX(aRotation.x) *
		CU::Matrix4x4f::CreateRotationAroundY(aRotation.y) *
		translation;

	myTransform = newMatrix;
}

void CModelInstance::SetPosition(const CU::Vector3f aPosition)
{
	myTransform(4, 1) = aPosition.x;
	myTransform(4, 2) = aPosition.y;
	myTransform(4, 3) = aPosition.z;
	myTransform(4, 4) = 1.0f;
}

void CModelInstance::Move(const CU::Vector3f aMovement)
{
	CU::Vector3f position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};
	
	position.x += aMovement.x;
	position.y += aMovement.y;
	position.z += aMovement.z;
	
	SetPosition(position);
}

void CModelInstance::Rotate(const CU::Vector3f aRotation)
{
	const CU::Vector3f position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	myTransform *= CU::Matrix4x4f::CreateRotationAroundX(aRotation.x);
	myTransform *= CU::Matrix4x4f::CreateRotationAroundY(aRotation.y);
	myTransform *= CU::Matrix4x4f::CreateRotationAroundZ(aRotation.z);

	SetPosition(position);
}
