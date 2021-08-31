#pragma once
#include "Vector.h"

class CModelInstance;

class CGameObject
{
public:
	CGameObject() = delete;
	CGameObject(const std::string& aName) : myName(aName) { };
	~CGameObject() = default;

	void SetName(const std::string& aName);
	void SetModelInstance(CModelInstance* aModelInstance);

	void SetTransform(const CU::Vector3f aPosition, CU::Vector3f aRotation);
	void SetRotation(const CU::Vector3f aRotation);
	void SetPosition(const CU::Vector3f aPosition);

	void Move(const CU::Vector3f aMovement);
	void Rotate(const CU::Vector3f aRotation);

public:
	const std::string&	GetName() { return myName; };
	const CU::Vector3f	GetPosition();
	const CU::Vector3f	GetRotation();
	const CU::Vector3f	GetScale();

private:
	std::string myName = "GameObject";

	CModelInstance* myModelInstance = nullptr;

	CU::Vector3f myPosition;
	CU::Vector3f myRotation;
	CU::Vector3f myScale;

};

