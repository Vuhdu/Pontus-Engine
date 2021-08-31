#pragma once
#include <Windows.h>

class CWindowHandler
{
public:
	struct SWindowData
	{
		int myX = 0;
		int myY = 0;
		int myWidth = 0;
		int myHeight = 0;
	};

	CWindowHandler();
	~CWindowHandler();

	static LRESULT CALLBACK WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	bool Init(SWindowData aWindowData);


	HWND GetHWND() const;

	int GetWidth() const;
	int GetHeight() const;

private:
	HWND myWindowHandle;
	SWindowData myWindowData;

};

