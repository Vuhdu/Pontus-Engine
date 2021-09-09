#pragma once
#include <functional>

class CWindowHandler;
class CDirectX11Framework;

namespace Editor 
{
	class CEditorInterface;

	class CEditor 
	{
	public:
		CEditor() = default;
		~CEditor();

		bool Init(CWindowHandler* aWindowHandler, CDirectX11Framework* aFrameWork);

		void BeginFrame(const bool anEditorToggle);
		void EndFrame(const bool anEditorToggle);

	private:
		CEditorInterface* myEditorInterface = nullptr;

	};
}