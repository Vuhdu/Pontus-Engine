#include "pch.h"
#include "Inspector.h"

#include "GameObject.h"

void Editor::CInspector::Draw()
{
	if (ImGui::Button("Add Component"))
	{
		CEngine::GetInstance()->SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	/*
	auto selectedGo = CEngine::GetInstance()->GetSelectedGameObject();

	if (!selectedGo)
	{
		return;
	}

	ImGui::Text("Name: %s", selectedGo->GetName().c_str());
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Transform"))
	{
		CU::Vector3f pos = selectedGo->GetPosition();
		CU::Vector3f rot = selectedGo->GetRotation();
		CU::Vector3f scale = selectedGo->GetScale();

		ImGui::Text("Position: [x: %.2f, y: %.2f, z: %.2f ]", pos.x, pos.y, pos.z);
		ImGui::Text("Rotation: [x: %.2f, y: %.2f, z: %.2f ]", rot.x, rot.y, rot.z);
		ImGui::Text("Scale: [x: %.1f, y: %.1f, z: %.1f ]", scale.x, scale.y, scale.z);
	}
	*/
}
