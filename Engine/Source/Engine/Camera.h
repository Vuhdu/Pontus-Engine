#pragma once

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

	void Init(const float aFieldOfView, const CU::Vector2ui aResolution, eOrientation anOrientation = eOrientation::Perspective);

	void SetTransform(const CU::Vector3f aPosition, CU::Vector3f aRotation);
	void SetRotation(const CU::Vector3f aRotation);
	void SetPosition(const CU::Vector3f aPosition);

	void Update(const float aDeltaTime);
	void HandleMovement(CU::InputHandler& anInputHandler, const float aDeltaTime);

	void Move(const CU::Vector3f aMovement);
	void Rotate(const CU::Vector3f aRotation);

public:
	CU::Vector3f GetPosition() { return { myTransform(4, 1), myTransform(4, 2), myTransform(4, 3) }; }

	CU::Matrix4x4f	GetTransform() { return myTransform; }
	CU::Matrix4x4f	GetProjection() { return myProjection; }

private:
	CU::Matrix4x4f myTransform;
	CU::Matrix4x4f myProjection;

	float myNearPlane = 0.1f;
	float myFarPlane = 100000.0f;

	CU::Vector2f myOldMousePosition = { 0.0f, 0.0f };


	float myPitch = 0.0f;
	float myYaw = 0.0f;
};

