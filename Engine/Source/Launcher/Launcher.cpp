#include "framework.h"
#include "Launcher.h"

#include "Engine.h"
#include "Game.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    hInstance;
    hPrevInstance;
    lpCmdLine;
    nCmdShow;

    CEngine* engine = new CEngine();
    if (!engine->Init())
    {
        return EXIT_FAILURE;
    }

    CGame* game = new CGame();
    if (!game->Init())
    {
        return EXIT_FAILURE;
    }

    bool shouldRun = true;
    MSG windowsMessage = { 0 };

    while (shouldRun)
    {
        while (PeekMessage(&windowsMessage, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&windowsMessage);
            DispatchMessage(&windowsMessage);

            if (windowsMessage.message == WM_QUIT)
            {
                shouldRun = false;
            }
        }
        engine->Update();
        game->Update();
    }
    game->Destroy();
    delete game;
    engine->Destroy();
    delete engine;

    return 0;
}
