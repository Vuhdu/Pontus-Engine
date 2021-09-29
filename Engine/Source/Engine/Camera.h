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
	CU::Vector3f GetPosition() { return { myTransform.GetPosition() }; }

	CU::Transform	GetTransform() { return myTransform; }
	CU::Matrix4f	GetProjection() { return myProjection; }

private:
	CU::Transform myTransform;
	CU::Matrix4f myProjection;

	float myNearPlane = 0.1f;
	float myFarPlane = 100000.0f;

	CU::Vector2f myOldMousePosition = { 0.0f, 0.0f };


	float myPitch = 0.0f;
	float myYaw = 0.0f;
};

