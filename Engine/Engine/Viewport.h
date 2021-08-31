#pragma once
#include "EditorWindow.h"

class CDirectX11Framework;
class CWindowHandler;

namespace Editor
{
	class CViewport : public CEditorWindow
	{
	public:
		CViewport(const std::string& aWindowName) : CEditorWindow(aWindowName) { };
		~CViewport() = default;

		void SetFramework(CDirectX11Framework* aFrameWork);
		void SetWindowHandler(CWindowHandler* aWindowHandler);
		void Draw() override;
		void Draw(void* aVoidPtr);

	protected:
		const ImVec2 ClampToAspectRatio(const ImVec2& aSizeToScale, const ImVec2& anAspectRatio) const;
		CDirectX11Framework* GetFramework() const;

	private:
		CDirectX11Framework* myFrameWork = nullptr;
		CWindowHandler* myWindowHandler = nullptr;

	};
}

