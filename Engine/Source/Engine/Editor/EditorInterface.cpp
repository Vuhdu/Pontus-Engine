#include "pch.h"
#include "EditorInterface.h"

#include "WindowHandler.h"
#include "DirectX11Framework.h"

#include "SceneViewport.h"
#include "GameViewport.h"
#include "Inspector.h"
#include "AssetBrowser.h"
#include "SceneHierarchy.h"
#include "DebugLog.h"

Editor::CEditorInterface::~CEditorInterface()
{
	Cleanup();
}

bool Editor::CEditorInterface::Init(CWindowHandler* aWindowHandler, CDirectX11Framework* aFrameWork)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SetDarkThemeColors();

	if (!ImGui_ImplWin32_Init(aWindowHandler->GetHWND()))
	{
		INFO_PRINT("ImGui_ImplWin32_Init failed");
		return false;
	}

	if (!ImGui_ImplDX11_Init(aFrameWork->GetDevice(), aFrameWork->GetContext()))
	{
		INFO_PRINT("ImGui_ImplDX11_Init failed");
		return false;
	}

	myWindowHandler = aWindowHandler;
	myFrameWork = aFrameWork;

	CreateSceneViewport();
	CreateGameViewport();
	CreateAssetBrowser();
	CreateInspector();
	CreateSceneHierarchy();
	CreateDebugLog();

	return true;
}

void Editor::CEditorInterface::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Editor::CEditorInterface::RenderEditorInterface()
{
	DrawInterface();

	if (myGUIWindows.size() > 0)
	{
		for (auto& window : myGUIWindows)
		{
			ImGui::Begin(window->GetWindowName().c_str(), &window->IsOpen(), window->GetImguiFlags());
			{
				window->Draw();
			}
			ImGui::End();
		}

		const int start = static_cast<int>(myGUIWindows.size() - 1);
		for (int windowIndex = start; windowIndex >= 0; --windowIndex)
		{
			auto& window = myGUIWindows[windowIndex];
			if (window->IsOpen() == false)
			{
				delete window;
				window = nullptr;
				myGUIWindows.erase(myGUIWindows.begin() + windowIndex);
			}
		}
	}
}

void Editor::CEditorInterface::DrawInterface()
{
	static bool p_open = true;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("EditorInterface", &p_open, window_flags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
				INFO_PRINT("New");
			}
			if (ImGui::MenuItem("Open", "CTRL + O"))
			{
				INFO_PRINT("Open");
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::BeginMenu("New Window..."))
			{
				if (ImGui::MenuItem("Inspector"))
				{
					CreateInspector();
				}
				if (ImGui::MenuItem("Scene View"))
				{
					CreateSceneViewport();
				}
				if (ImGui::MenuItem("Game View"))
				{
					CreateGameViewport();
				}
				if (ImGui::MenuItem("Scene Hierarchy"))
				{
					CreateSceneHierarchy();
				}
				if (ImGui::MenuItem("Asset Browser"))
				{
					CreateAssetBrowser();
				}
				if (ImGui::MenuItem("Debug Log"))
				{
					CreateDebugLog();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Render Mode"))
			{
				if (ImGui::MenuItem("Solid"))
				{
					CEngine::SetRenderMode(eRenderMode::Solid);
				}
				if (ImGui::MenuItem("Wireframe"))
				{
					CEngine::SetRenderMode(eRenderMode::Wireframe);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Skins"))
			{
				if (ImGui::MenuItem("Darkmode"))
				{
					SetDarkThemeColors();
				}
				if (ImGui::MenuItem("Lightmode"))
				{
					SetLightThemeColors();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void Editor::CEditorInterface::EndFrame()
{
	if (myWillRender == true)
	{
		RenderEditorInterface();
	}

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImGui::GetMainViewport()->WorkSize;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Editor::CEditorInterface::Cleanup()
{
	if (myGUIWindows.size() > 0)
	{
		for (auto& window : myGUIWindows)
		{
			delete window;
			window = nullptr;
		}
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Editor::CEditorInterface::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 0.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

void Editor::CEditorInterface::SetLightThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.8f, 0.85f, 0.81f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.7f, 0.75f, 0.71f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.8f, 0.805f, 0.81f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.7f, 0.705f, 0.71f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.8f, 0.805f, 0.81f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.7f, 0.705f, 0.71f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.68f, 0.6805f, 0.681f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.78f, 0.7805f, 0.781f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.8f, 0.805f, 0.81f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
}

void Editor::CEditorInterface::CreateSceneHierarchy()
{
	auto hierarchy = TryCreateWindow<CSceneHierarchy>("Scene Hierarchy");

	hierarchy->SetActiveScene(CEngine::GetScene());

	myGUIWindows.push_back(hierarchy);
}

void Editor::CEditorInterface::CreateSceneViewport()
{
	auto viewport = TryCreateWindow<CSceneViewport>("Scene View");

	viewport->AddFlag(eEditorWindowFlags::GetMouseInput);
	viewport->AddFlag(eEditorWindowFlags::AllowOnlyOneInstance);

	TestFlags(viewport);

	viewport->SetImguiFlags(ImGuiWindowFlags_NoCollapse);
	viewport->SetFramework(myFrameWork);
	viewport->SetWindowHandler(myWindowHandler);

	myGUIWindows.push_back(viewport);
}

void Editor::CEditorInterface::CreateGameViewport()
{
	auto viewport = TryCreateWindow<CGameViewport>("Game View");

	viewport->AddFlag(eEditorWindowFlags::AllowOnlyOneInstance);

	TestFlags(viewport);

	viewport->SetImguiFlags(ImGuiWindowFlags_NoCollapse);
	viewport->SetFramework(myFrameWork);
	viewport->SetWindowHandler(myWindowHandler);

	myGUIWindows.push_back(viewport);
}

void Editor::CEditorInterface::CreateAssetBrowser()
{
	auto browser = TryCreateWindow<CAssetBrowser>("Asset Browser");

	myGUIWindows.push_back(browser);
}

void Editor::CEditorInterface::CreateInspector()
{
	auto inspector = TryCreateWindow<CInspector>("Inspector");

	myGUIWindows.push_back(inspector);
}

void Editor::CEditorInterface::CreateDebugLog()
{
	auto debuglog = TryCreateWindow<CDebugLog>("Debug Log");

	myGUIWindows.push_back(debuglog);
}
