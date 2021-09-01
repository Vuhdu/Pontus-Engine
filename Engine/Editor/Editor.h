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

		void BeginFrame();
		void EndFrame();

	private:
		CEditorInterface* myEditorInterface = nullptr;

	};
}