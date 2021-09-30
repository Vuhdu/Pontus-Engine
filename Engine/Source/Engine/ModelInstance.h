#pragma once
#include "Transform.h"

class CModel;

class CModelInstance
{
public:
	CModelInstance();
	~CModelInstance();

	void Init(const std::vector<CModel*>& aModelVector);

	void SetTransform(const CU::Transform& aTransform);
	void SetRotation(const CU::Vector3f aRotation);
	void SetPosition(const CU::Vector3f aPosition);
	void SetScale(const CU::Vector3f aScale);

	void Move(const CU::Vector3f aMovement);
	void Rotate(const CU::Vector3f aRotation);

public:
	const CU::Vector3f&				GetPosition() const { return myTransform.GetPosition(); }
	const CU::Vector3f&				GetRotation() const { return myTransform.GetRotation(); }
	const CU::Vector3f&				GetScale() const { return myTransform.GetScale(); }

	const std::vector<CModel*>&		GetModelVector() const { return myModelVector; }
	const CU::Transform&			GetTransform() const { return myTransform; }

private:
	std::vector<CModel*> myModelVector;

	CU::Transform myTransform;

};

