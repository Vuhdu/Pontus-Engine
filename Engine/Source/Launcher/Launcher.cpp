#include "Engine.h"
#include "Game.h"
#include "CreateParameters.h"

#ifndef _RELEASE
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <string.h>

#include <../Engine/DirectXTK/Inc/ScreenGrab.h>
#include <../Engine/DirectX11Framework.h>

#include <wrl/client.h>
#include <Wincodec.h>
#include <d3d11.h>

#include <filesystem>

void CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers, MINIDUMP_TYPE someFlags)
{
    HRESULT error = S_OK;
    SYSTEMTIME sysTime = { 0 };
    GetSystemTime(&sysTime);

    char compName[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
    DWORD compNameLen = ARRAYSIZE(compName);
    GetComputerNameA(compName, &compNameLen);

    // Get full path
    char path[MAX_PATH] = { 0 };
    char pBuf[512];
    std::string cPath(pBuf, GetModuleFileNameA(NULL, pBuf, MAX_PATH));

    std::string cApplicationName = "Launcher_Release.exe";
    cPath.resize(cPath.size() - cApplicationName.size());
    cPath.append("Crashes\\");

    std::string dumpFile = "Program crashes! A minidump was created at \"";
    dumpFile.append(cPath);

    // Name of the crash log
    char name[MAX_PATH] = { 0 };
    sprintf_s(name, ARRAYSIZE(name),
        "\\%s_%04u-%02u-%02u_%02u-%02u-%02u",
        compName, sysTime.wYear, sysTime.wMonth, sysTime.wDay,
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

    // Create subfolder
    std::string newSubfolder = "Crashes";
    newSubfolder.append(name);

    if (!std::filesystem::exists(newSubfolder))
    {
        std::filesystem::create_directories(newSubfolder);
    }

    dumpFile.append(name);
    dumpFile.append("\", please tell a programmer.");

    cPath.append(name);
    cPath.append("\\%s_%04u-%02u-%02u_%02u-%02u-%02u.dmp");

    sprintf_s(path, ARRAYSIZE(path),
        cPath.c_str(),
        compName, sysTime.wYear, sysTime.wMonth, sysTime.wDay,
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

    HANDLE hFile = CreateFileA(path,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        error = GetLastError();
        error = HRESULT_FROM_WIN32(error);
        return;
    }

    HANDLE hProc = GetCurrentProcess();
    DWORD procID = GetProcessId(hProc);

    MINIDUMP_EXCEPTION_INFORMATION sehInfo = { 0 };
    MINIDUMP_EXCEPTION_INFORMATION* sehPtr = NULL;

    if (someExceptionPointers)
    {
        sehInfo.ThreadId = GetCurrentThreadId();
        sehInfo.ExceptionPointers = someExceptionPointers;
        sehInfo.ClientPointers = FALSE;
        sehPtr = &sehInfo;
    }

    BOOL result = MiniDumpWriteDump(hProc, procID, hFile, someFlags, sehPtr, NULL, NULL);

    if (!result)
    {
        error = (HRESULT)GetLastError();
    }
    CloseHandle(hFile);


    // Print screenshot
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    auto swapchain = CEngine::GetFramework()->GetSwapChain();
    HRESULT hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));

    if (SUCCEEDED(hr))
    {
        std::wstring path;
        path.append(L"Crashes\\");

        wchar_t* wName;
        int nChars = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, 0);
        wName = new wchar_t[nChars];
        MultiByteToWideChar(CP_ACP, 0, name, -1, (LPWSTR)wName, nChars);
        path.append(wName);
        path.append(L"\\");
        path.append(wName);
        delete wName;

        path.append(L".JPG");

        auto context = CEngine::GetFramework()->GetContext();
        hr = DirectX::SaveWICTextureToFile(context, backBuffer.Get(), GUID_ContainerFormatJpeg, path.c_str());
    }

    MessageBoxA(nullptr, dumpFile.c_str(), "FATAL ERROR", MB_OK | MB_ICONERROR);
}

LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* anException)
{
    if (!std::filesystem::exists("Crashes"))
    {
        std::filesystem::create_directories("Crashes");
    }

    CreateMiniDump(anException, MINIDUMP_TYPE::MiniDumpNormal);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int RunProgram()
{
#ifndef _DEBUG
    __try
    {
        return CEngine::Run();
    }
    __except (ExceptionFilterFunction(GetExceptionInformation()))
    {

    }
#else
    return CEngine::Run();
#endif
}

int main() 
{
    SCreateParameters createParameters;

    CGame* game = new CGame(&createParameters);

    if (!CEngine::Start(&createParameters))
    {
        return EXIT_FAILURE;
    }
    return RunProgram();
}