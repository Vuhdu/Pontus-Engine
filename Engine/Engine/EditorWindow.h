#pragma once
#include <bitset>

namespace Editor
{
	enum class eEditorWindowFlags
	{
		None = 0,				// No flags.
		AllowOnlyOneInstance,	// Only one copy of this window can be opened at once.
		GetMouseInput,			// Allow editor window to detect if cursor is above window.

		Count
	};

	// template<class T> inline T operator~ (T a) { return (T)~(int)a; }
	// template<class T> inline T operator| (T a, T b) { return (T)((int)a | (int)b); }
	// template<class T> inline T operator& (T a, T b) { return (T)((int)a & (int)b); }
	// template<class T> inline T operator^ (T a, T b) { return (T)((int)a ^ (int)b); }
	// template<class T> inline T& operator|= (T& a, T b) { return (T&)((int&)a |= (int)b); }
	// template<class T> inline T& operator&= (T& a, T b) { return (T&)((int&)a &= (int)b); }
	// template<class T> inline T& operator^= (T& a, T b) { return (T&)((int&)a ^= (int)b); }

	class CEditorWindow
	{
	public:
		CEditorWindow() = delete;
		CEditorWindow(const std::string& aWindowName) : myWindowName(aWindowName) 
		{

		};
		virtual ~CEditorWindow() { };

		virtual void Draw() = 0;

		void SetImguiFlags(const ImGuiWindowFlags someFlags);

		void AddFlag(const eEditorWindowFlags& anEditorWindowFlag)
		{
			myWindowFlags[static_cast<int>(anEditorWindowFlag)].flip();
		};

	public:
		const std::bitset<static_cast<int>(eEditorWindowFlags::Count)>& GetEditorWindowFlags() const { return myWindowFlags; };
		const ImGuiWindowFlags& GetImguiFlags() const { return myImguiFlags; };
		bool& IsOpen() { return myIsOpen; };
		const std::string& GetWindowName() const { return myWindowName; };

		const bool IsCursorHovering() const { return ImGui::IsWindowHovered(ImGuiHoveredFlags_None); };

	protected:
		bool myIsOpen = true;

	private:
		std::bitset<static_cast<int>(eEditorWindowFlags::Count)> myWindowFlags;

		ImGuiWindowFlags myImguiFlags = ImGuiWindowFlags_None;

		std::string myWindowName;
	};
}
