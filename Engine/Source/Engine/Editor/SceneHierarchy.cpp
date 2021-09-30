#include "pch.h"
#include "SceneHierarchy.h"

#include "Scene.h"

void Editor::CSceneHierarchy::Draw()
{
	return;
	/*

	auto selectedGo = CEngine::GetInstance()->GetSelectedGameObject();
	auto go = myActiveScene->GetGameObjects();

	for (int i = 0; i < go.size(); i++)
	{
		ImGuiTreeNodeFlags flags = ((selectedGo == go[i]) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool isOpen = ImGui::TreeNodeEx((void*)go[i], flags, go[i]->GetName().c_str());

		if (ImGui::IsItemClicked())
		{
			CEngine::GetInstance()->SetSelectedGameObject(go[i]);
		}

		ImGui::PushID(i);
		if (isOpen)
		{
			if (ImGui::IsItemToggledOpen())
			{
				INFO_PRINT("Toggled \"%s\" open", go[i]->GetName().c_str());
			}

			for (int ii = 0; ii < 3; ii++)
			{
				ImGui::PushID(ii);
				if (ImGui::TreeNode("Child"))
				{
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	*/
}

void Editor::CSceneHierarchy::SetActiveScene(CScene* anActiveScene)
{
	myActiveScene = anActiveScene;
}
