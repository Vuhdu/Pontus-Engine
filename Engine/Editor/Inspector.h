#pragma once
#include "EditorWindow.h"

namespace Editor
{
	class CInspector : public CEditorWindow
	{
	public:
		CInspector(const std::string& aWindowName) : CEditorWindow(aWindowName) { };
		~CInspector() = default;

		void Draw() override;

	private:

	};
}

