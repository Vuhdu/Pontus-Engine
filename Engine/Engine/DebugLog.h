#pragma once
#include "EditorWindow.h"

namespace Editor
{
	class CDebugLog : public CEditorWindow
	{
	public:
		CDebugLog(const std::string& aWindowName) : CEditorWindow(aWindowName) { };
		~CDebugLog() = default;

		void Draw() override;

	private:

	};
}
