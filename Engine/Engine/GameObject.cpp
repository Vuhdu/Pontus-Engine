#include "stdafx.h"
#include "GameObject.h"

#include "ModelInstance.h"

void CGameObject::SetName(const std::string& aName)
{
	myName = aName;
}

void CGameObject::SetModelInstance(CModelInstance* aModelInstance)
{
	myModelInstance = aModelInstance;

	auto& transform = myModelInstance->GetTransform();
	myPosition = { transform(4, 1), transform(4, 2), transform(4, 3) };
	myRotation = transform.EulerAngles();
	myScale = { 1.0f, 1.0f, 1.0f };
}

void CGameObject::SetTransform(const CU::Vector3f aPosition, CU::Vector3f aRotation)
{
	myPosition = aPosition;
	myRotation = aRotation;
	if (myModelInstance)
	{
		myModelInstance->SetTransform(aPosition, aRotation);
	}
}

void CGameObject::SetRotation(const CU::Vector3f aRotation)
{
	myRotation = aRotation;
	if (myModelInstance)
	{
		myModelInstance->SetRotation(aRotation);
	}
}

void CGameObject::SetPosition(const CU::Vector3f aPosition)
{
	myPosition = aPosition;
	if (myModelInstance)
	{
		myModelInstance->SetPosition(aPosition);
	}
}

void CGameObject::Move(const CU::Vector3f aMovement)
{
	if (myModelInstance)
	{
		myModelInstance->Move(aMovement);

		myPosition.x = myModelInstance->GetTransform()(4, 1);
		myPosition.y += myModelInstance->GetTransform()(4, 2);
		myPosition.z += myModelInstance->GetTransform()(4, 3);
	}
	else
	{
		myPosition.x += aMovement.x;
		myPosition.y += aMovement.y;
		myPosition.z += aMovement.z;
	}
}

void CGameObject::Rotate(const CU::Vector3f aRotation)
{
	if (myModelInstance)
	{
		myModelInstance->Rotate(aRotation);
		myRotation = myModelInstance->GetTransform().EulerAngles();
	}
	else
	{
		myRotation.x += aRotation.x;
		myRotation.y += aRotation.y;
		myRotation.z += aRotation.z;
	}
}

const CU::Vector3f CGameObject::GetPosition()
{
	if (myModelInstance)
	{
		auto& transform = myModelInstance->GetTransform();
		return { transform(4, 1), transform(4, 2), transform(4, 3) };
	}
	else
	{
		return myPosition;
	}
}

const CU::Vector3f CGameObject::GetRotation()
{
	if (myModelInstance)
	{
		auto& transform = myModelInstance->GetTransform();
		return transform.EulerAngles();
	}
	else
	{
		return myRotation;
	}
}

const CU::Vector3f CGameObject::GetScale()
{
	return myScale;
}
