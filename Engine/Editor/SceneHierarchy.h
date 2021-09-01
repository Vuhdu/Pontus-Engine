#pragma once
#include "EditorWindow.h"

class CScene;

namespace Editor
{
	class CSceneHierarchy : public CEditorWindow
	{
	public:
		CSceneHierarchy(const std::string& aWindowName) : CEditorWindow(aWindowName) { };
		~CSceneHierarchy() = default;

		void Draw() override;

		void SetActiveScene(CScene* anActiveScene);

	private:
		CScene* myActiveScene = nullptr;

	};
}

