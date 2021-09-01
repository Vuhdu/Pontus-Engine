#pragma once
#include "EditorWindow.h"

namespace Editor
{
	class CAssetBrowser : public CEditorWindow
	{
	public:
		CAssetBrowser(const std::string& aWindowName) : CEditorWindow(aWindowName) { };
		~CAssetBrowser() = default;

		void Draw() override;

	private:

	};
}
