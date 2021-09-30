#pragma once
#include "Transform.h"

namespace CommonUtilities
{
	class InputHandler;
}

enum class eOrientation
{
	Perspective,
	Orthographic
};

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(
		const float aFieldOfView, 
		const float aNearPlane,
		const float aFarPlane,
		const CU::Vector2ui aResolution, 
		eOrientation anOrientation = eOrientation::Perspective);

	void SetTransform(const CU::Transform& aTransform);
	void SetRotation(const CU::Vector3f aRotation);
	void SetPosition(const CU::Vector3f aPosition);

	void Update(const float aDeltaTime);
	void HandleMovement(CU::InputHandler& anInputHandler, const float aDeltaTime);

	void Move(const CU::Vector3f aMovement);
	void Rotate(const CU::Vector3f aRotation);

public:
	CU::Vector3f GetPosition() { return { myTransform.GetPosition() }; }

	CU::Transform	GetTransform() { return myTransform; }
	CU::Matrix4x4f	GetProjection() { return myProjection; }

private:
	CU::Transform myTransform;
	CU::Matrix4x4f myProjection;

	float myNearPlane = 1.0f;
	float myFarPlane = 100000.0f;

	CU::Vector2f myOldMousePosition = { 0.0f, 0.0f };


	float myPitch = 0.0f;
	float myYaw = 0.0f;
};

