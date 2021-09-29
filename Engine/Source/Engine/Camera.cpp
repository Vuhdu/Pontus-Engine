#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Init(const float aFieldOfView, const CU::Vector2ui aResolution, eOrientation anOrientation)
{
	if (anOrientation == eOrientation::Perspective)
	{
		const float hFoVRad = aFieldOfView * (CU_PI / 180.f);
		const float vFoVRad = 2.f * std::atan(std::tan(hFoVRad / 2.f) * static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x));

		const float vFoVDeg = std::ceil(vFoVRad * (180.f / CU_PI));
		vFoVDeg;

		const float scaleX = 1.f / std::tan(hFoVRad * .5f);
		const float scaleY = 1.f / std::tan(vFoVRad * .5f);

		const float Q = myFarPlane / (myFarPlane - myNearPlane);

		myProjection(1, 1) = scaleX;
		myProjection(2, 2) = scaleY;
		myProjection(3, 3) = Q;
		myProjection(3, 4) = 1.f / Q;
		myProjection(4, 3) = -Q * myNearPlane;
		myProjection(4, 4) = 0.0f;
	}
	else 
	{
		myProjection(1, 1) = 2.0f / static_cast<float>(aResolution.x);
		myProjection(2, 2) = 2.0f / static_cast<float>(aResolution.y);
		myProjection(3, 3) = 1.0f / (myFarPlane - myNearPlane);
		myProjection(4, 3) = myNearPlane / (myNearPlane - myFarPlane);
		myProjection(4, 4) = 1.0f;
	}
}

void CCamera::SetTransform(const CU::Vector3f aPosition, const CU::Vector3f aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CCamera::SetRotation(const CU::Vector3f aRotation)
{
	myTransform.SetRotationRad(aRotation);
}

void CCamera::SetPosition(const CU::Vector3f aPosition)
{
	myTransform(4, 1) = aPosition.x;
	myTransform(4, 2) = aPosition.y;
	myTransform(4, 3) = aPosition.z;
	myTransform(4, 4) = 1.0f;
}

void CCamera::Update(const float aDeltaTime)
{
	aDeltaTime;
}

void CCamera::HandleMovement(CU::InputHandler& anInputHandler, const float aDeltaTime)
{
	const CU::Vector3f right = myTransform.Right();
	const CU::Vector3f up = myTransform.Up();
	const CU::Vector3f forward = myTransform.Forward();

	CU::Vector3f position = { 0.0f, 0.0f, 0.0f };

	const float speed = (anInputHandler.IsKeyPressed(CU::eKeyCode::LeftShift) == true) ? 100.0f : 25.0f;
	const float rotationSpeed = speed * .1f;

	if (anInputHandler.IsKeyPressed(CU::eKeyCode::W))
	{
		position += forward * speed * aDeltaTime;
	}
	if (anInputHandler.IsKeyPressed(CU::eKeyCode::S))
	{
		position -= forward * speed * aDeltaTime;
	}
	if (anInputHandler.IsKeyPressed(CU::eKeyCode::A))
	{
		position -= right * speed * aDeltaTime;
	}
	if (anInputHandler.IsKeyPressed(CU::eKeyCode::D))
	{
		position += right * speed * aDeltaTime;
	}
	if (anInputHandler.IsKeyPressed(CU::eKeyCode::E))
	{
		position += up * speed * aDeltaTime;
	}
	if (anInputHandler.IsKeyPressed(CU::eKeyCode::Q))
	{
		position -= up * speed * aDeltaTime;
	}

	if (anInputHandler.IsKeyPressed(CU::eMouseButton::Right))
	{
		auto io = ImGui::GetIO();

		const float mouseSensitivity = 0.005f;
		myYaw += io.MouseDelta.x * mouseSensitivity;

		myPitch += io.MouseDelta.y * mouseSensitivity;
		myPitch = CU::Clamp(myPitch, -(CU_PI * 0.5f) + 0.001f, (CU_PI * 0.5f) - 0.001f);

		SetRotation({ myPitch, myYaw, 0.0f });
	}

	Move(position);
}

void CCamera::Move(const CU::Vector3f aMovement)
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

void CCamera::Rotate(const CU::Vector3f aRotation)
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