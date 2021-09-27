#pragma once

class CModel;

class CModelInstance
{
public:
	CModelInstance();
	~CModelInstance();

	void Init(const std::vector<CModel*>& aModelVector);

	void SetTransform(const CU::Vector3f aPosition, CU::Vector3f aRotation);
	void SetRotation(const CU::Vector3f aRotation);
	void SetPosition(const CU::Vector3f aPosition);
	void SetScale(const CU::Vector3f aScale);

	void Move(const CU::Vector3f aMovement);
	void Rotate(const CU::Vector3f aRotation);

public:
	const CU::Vector3f GetPosition() const { return CU::Vector3f{ myTransform(4, 1), myTransform(4, 2), myTransform(4, 3) }; };
	const std::vector<CModel*>&		GetModelVector() const { return myModelVector; };
	const CU::Matrix4x4f&			GetTransform() const { return myTransform; };

private:
	std::vector<CModel*> myModelVector;

	CU::Matrix4x4f myTransform;

};

