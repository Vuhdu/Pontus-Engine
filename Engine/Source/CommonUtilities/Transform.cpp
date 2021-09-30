#include "pch.h"
#include "Transform.h"

const CU::Vector3f& CU::Transform::GetPosition() const
{
	return myPosition;
}

void CU::Transform::SetPosition(const CU::Vector3f& aPosition)
{
	myPosition = aPosition;
}

const CU::Vector3f& CU::Transform::GetRotation() const
{
	return myRotation;
}

void CU::Transform::SetRotation(const CU::Vector3f& aRotation)
{
	myRotation = aRotation;
}

const CU::Vector3f& CU::Transform::GetScale() const
{
	return myScale;
}

void CU::Transform::SetScale(const float aScale)
{
	myScale = CU::Vector3f(aScale, aScale, aScale);
}

void CU::Transform::SetScale(const CU::Vector3f& aScale)
{
	myScale = aScale;
}

CU::Matrix4x4f CU::Transform::ToMatrix() const
{
	// MUST BE IN THIS ORDER
	// S * R * T
	// https://gamedev.stackexchange.com/questions/16719/what-is-the-correct-order-to-multiply-scale-rotation-and-translation-matrices-f

	return CU::Matrix4x4f::CreateScale(myScale)
		* CU::Matrix4x4f::CreateRotationAroundX(myRotation.x)
		* CU::Matrix4x4f::CreateRotationAroundY(myRotation.y)
		* CU::Matrix4x4f::CreateRotationAroundZ(myRotation.z)
		* CU::Matrix4x4f::CreateTranslation(myPosition);
}