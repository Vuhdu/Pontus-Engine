#include "pch.h"
#include "WindowHandler.h"
#include "MainSingleton.h"
#include "InputHandler.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CWindowHandler::CWindowHandler()
{
    myWindowHandle = HWND();
}

CWindowHandler::~CWindowHandler()
{
}

LRESULT CALLBACK CWindowHandler::WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
    {
        return true;
    }

    static CWindowHandler* windowHandler = nullptr;

    if (MainSingleton::GetInstance()->GetInput()->UpdateEvents(uMsg, wParam, lParam))
    {
        return 0;
    }

    if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
    {
        PostQuitMessage(0);
        return 0;
    }
    else if (uMsg == WM_CREATE)
    {
        CREATESTRUCT* createstruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        windowHandler = reinterpret_cast<CWindowHandler*>(createstruct->lpCreateParams);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool CWindowHandler::Init(SWindowData aWindowData)
{
    WNDCLASS windowclass = {};
    windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    windowclass.lpfnWndProc = CWindowHandler::WinProc;
    windowclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowclass.lpszClassName = L"TillämpadGrafikprogrammering";
    RegisterClass(&windowclass);

    myWindowData = aWindowData;

    myWindowHandle = CreateWindow(L"TillämpadGrafikprogrammering", L"My Awesome Game",
        WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
        aWindowData.myX, aWindowData.myY, aWindowData.myWidth, aWindowData.myHeight,
        nullptr, nullptr, nullptr, this);

    return true;
}

HWND CWindowHandler::GetHWND() const
{
    return myWindowHandle;
}

int CWindowHandler::GetWidth() const
{
    return myWindowData.myWidth;
}

int CWindowHandler::GetHeight() const
{
    return myWindowData.myHeight;
}
