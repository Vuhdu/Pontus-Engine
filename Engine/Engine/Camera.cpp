#include "stdafx.h"
#include "Camera.h"

#include "MainSingleton.h"

#include "Mathcu.hpp"
#include "InputHandler.h"


CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Init(const float aFieldOfView, const CU::Vector2f aResolution)
{
	const float hFoVRad = aFieldOfView * (CU_PI / 180.f);
	const float vFoVRad = 2.f * std::atan(std::tan(hFoVRad / 2.f) * (aResolution.y / aResolution.x));

	const float vFoVDeg = std::ceil(vFoVRad * (180.f / CU_PI));
	vFoVDeg;

	const float scaleX = 1.f / std::tan(hFoVRad * .5f);
	const float scaleY = 1.f / std::tan(vFoVRad * .5f);

	const float Q = myFarPlane / (myFarPlane - myNearPlane);

	myProjection(1, 1) = scaleX;
	myProjection(2, 2) = scaleY;
	myProjection(3, 3) = Q;
	myProjection(3, 4) = 1.f / Q;
	myProjection(4, 3) = -Q  * myNearPlane;
	myProjection(4, 4) = 0.0f;
}

void CCamera::SetTransform(const CU::Vector3f aPosition, const CU::Vector3f aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CCamera::SetRotation(const CU::Vector3f aRotation)
{
	const CU::Vector3f position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	const CU::Matrix4x4f rotation =
		CU::Matrix4x4f::CreateRotationAroundX(aRotation.x) *
		CU::Matrix4x4f::CreateRotationAroundY(aRotation.y) *
		CU::Matrix4x4f::CreateRotationAroundZ(aRotation.z);

	myTransform(1, 1) = rotation(1, 1);
	myTransform(1, 2) = rotation(1, 2);
	myTransform(1, 3) = rotation(1, 3);

	myTransform(2, 1) = rotation(2, 1);
	myTransform(2, 2) = rotation(2, 2);
	myTransform(2, 3) = rotation(2, 3);

	myTransform(3, 1) = rotation(3, 1);
	myTransform(3, 2) = rotation(3, 2);
	myTransform(3, 3) = rotation(3, 3);

	SetPosition(position);
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
	// DebugDrawImgui();
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

void CCamera::DebugDrawImgui()
{
	const float PAD = 10.0f;
	static int corner = 0;

	bool p_open = true;
	auto flags = 
		ImGuiWindowFlags_NoDecoration | 
		ImGuiWindowFlags_AlwaysAutoResize | 
		ImGuiWindowFlags_NoSavedSettings | 
		ImGuiWindowFlags_NoFocusOnAppearing | 
		ImGuiWindowFlags_NoNav;

	if (corner != -1)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos;
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;

		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		flags |= ImGuiWindowFlags_NoMove;
	}

	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("CameraOverlay", &p_open, flags);
	{
		ImGui::Text("Camera");
		ImGui::Separator();
		ImGui::Text("Movement: WASD");
		ImGui::Text("Rotate: Arrow keys");
		ImGui::Separator();

		const CU::Vector3f position = myTransform.Position();
		const CU::Vector3f eulerAngles = myTransform.EulerAngles();
		const CU::Vector3f scale = myTransform.Scale();

		ImGui::Text("Position: [x: %.f, y: %.f, z: %.f]", position.x, position.y, position.z);
		ImGui::Text("Rotation: [x: %.f, y: %.f, z: %.f]", eulerAngles.x, eulerAngles.y, eulerAngles.z);
		ImGui::Text("Scale: [x: %.1f, y: %.1f, z: %.1f]", scale.x, scale.y, scale.z);

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}
