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

void CModelInstance::SetScale(const CU::Vector3f aScale)
{
	CU::Vector3f x = { myTransform(1, 1), myTransform(1, 2), myTransform(1,3) };
	x.Normalize();
	x *= aScale.x;
	myTransform(1, 1) = x.x;
	myTransform(1, 2) = x.y;
	myTransform(1, 3) = x.z;

	CU::Vector3f y = { myTransform(2, 1), myTransform(2, 2), myTransform(2,3) };
	y.Normalize();
	y *= aScale.y;
	myTransform(2, 1) = y.x;
	myTransform(2, 2) = y.y;
	myTransform(2, 3) = y.z;

	CU::Vector3f z = { myTransform(3, 1), myTransform(3, 2), myTransform(3,3) };
	z.Normalize();
	z *= aScale.z;
	myTransform(3, 1) = z.x;
	myTransform(3, 2) = z.y;
	myTransform(3, 3) = z.z;
}

void CModelInstance::SetRotation(const CU::Vector3f aRotation)
{
	myTransform.SetRotationRad(aRotation);
	/*CU::Matrix4x4f translation;
	translation(4, 1) = myTransform(4, 1);
	translation(4, 2) = myTransform(4, 2);
	translation(4, 3) = myTransform(4, 3);

	const CU::Matrix4x4f newMatrix =
		CU::Matrix4x4f::CreateRotationAroundZ(aRotation.z) *
		CU::Matrix4x4f::CreateRotationAroundX(aRotation.x) *
		CU::Matrix4x4f::CreateRotationAroundY(aRotation.y) *
		translation;

	myTransform = newMatrix;*/
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
