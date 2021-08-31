#pragma once
#include "EditorWindow.h"

class CWindowHandler;
class CDirectX11Framework;

namespace Editor
{
	class CEditorInterface
	{
	public:
		CEditorInterface() = default;
		~CEditorInterface();

		bool Init(CWindowHandler* aWindowHandler, CDirectX11Framework* aFrameWork);
	
		void BeginFrame();
		void EndFrame();

		void RenderEditorInterface();
		void DrawInterface();

		void Cleanup();

	private:
		void SetDarkThemeColors();
		void SetLightThemeColors();
		
		void CreateSceneHierarchy();
		void CreateSceneViewport();
		void CreateGameViewport();
		void CreateAssetBrowser();
		void CreateInspector();
		void CreateDebugLog();

		template<class T>
		T* TryCreateWindow(const std::string& aWindowName);

		void TestFlags(CEditorWindow* anEditorWindow)
		{
			auto HasFlag = [&anEditorWindow](const eEditorWindowFlags& anEditorWindowFlag) -> bool
			{
				return anEditorWindow->GetEditorWindowFlags().test(static_cast<int>(anEditorWindowFlag));
			};

			if (HasFlag(eEditorWindowFlags::None))
			{
				INFO_PRINT("%s contained flag: None", anEditorWindow->GetWindowName().c_str());
			}

			if (HasFlag(eEditorWindowFlags::AllowOnlyOneInstance))
			{
				INFO_PRINT("%s contained flag: AllowOnlyOneInstance", anEditorWindow->GetWindowName().c_str());
			}

			if (HasFlag(eEditorWindowFlags::GetMouseInput))
			{
				INFO_PRINT("%s contained flag: GetMouseInput", anEditorWindow->GetWindowName().c_str());
			}
		};

	private:
		CWindowHandler* myWindowHandler = nullptr;
		CDirectX11Framework* myFrameWork = nullptr;

		std::vector<CEditorWindow*> myGUIWindows;

		bool myWillRender = true;

	};


	template<class T>
	inline T* CEditorInterface::TryCreateWindow(const std::string& aWindowName)
	{
		T* window = new T(aWindowName);

		if (auto editorWindow = dynamic_cast<CEditorWindow*>(window))
		{
			auto HasFlag = [&editorWindow](eEditorWindowFlags anEditorWindowFlag) -> bool
			{
				return editorWindow->GetEditorWindowFlags().test(static_cast<int>(anEditorWindowFlag));

				//return editorWindow->GetEditorWindowFlags() == anEditorWindowFlag;
			};

			if (HasFlag(eEditorWindowFlags::None))
			{
				INFO_PRINT("%s contained flag: None", aWindowName.c_str());
			}

			if (HasFlag(eEditorWindowFlags::AllowOnlyOneInstance))
			{
				INFO_PRINT("%s contained flag: AllowOnlyOneInstance", aWindowName.c_str());
			}

			if (HasFlag(eEditorWindowFlags::GetMouseInput))
			{
				INFO_PRINT("%s contained flag: GetMouseInput", aWindowName.c_str());
			}
		}

		return window;
	}
}
