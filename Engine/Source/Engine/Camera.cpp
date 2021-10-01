#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Init(const float aFieldOfView, const float aNearPlane, const float aFarPlane, const CU::Vector2ui aResolution, eOrientation anOrientation)
{
	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;

	if (anOrientation == eOrientation::Perspective)
	{
		const float hFoVRad = aFieldOfView * (CU_PI / 180.f);
		const float vFoVRad = 2.f * std::atan(std::tan(hFoVRad / 2.f) * static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x));

		const float scaleX = 1.f / std::tanf(hFoVRad * .5f);
		const float scaleY = 1.f / std::tanf(vFoVRad * .5f);

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

void CCamera::SetTransform(const CU::Transform& aTransform)
{
	myTransform = aTransform;
}

void CCamera::SetRotation(const CU::Vector3f aRotation)
{
	myTransform.SetRotation(aRotation);
}

void CCamera::SetPosition(const CU::Vector3f aPosition)
{
	myTransform.SetPosition(aPosition);
}

void CCamera::Update(const float aDeltaTime)
{
	aDeltaTime;
}

void CCamera::HandleMovement(CU::InputHandler& anInputHandler, const float aDeltaTime)
{
	const CU::Vector3f right = myTransform.ToMatrix().GetRight();
	const CU::Vector3f up = myTransform.ToMatrix().GetUp();
	const CU::Vector3f forward = myTransform.ToMatrix().GetForward();

	CU::Vector3f position = { 0.0f, 0.0f, 0.0f };

	const float speed = (anInputHandler.IsKeyPressed(CU::eKeyCode::LeftShift) == true) ? 500.0f : 50.0f;
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
	myTransform.SetPosition(myTransform.GetPosition() + aMovement);
}

void CCamera::Rotate(const CU::Vector3f aRotation)
{
	myTransform.SetRotation(myTransform.GetRotation() + aRotation);
}