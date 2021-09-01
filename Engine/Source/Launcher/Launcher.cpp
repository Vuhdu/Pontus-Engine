#include "Engine.h"
#include "Game.h"
#include "CreateParameters.h"

int main() 
{
    SCreateParameters createParameters;

    CEngine* engine = new CEngine();
    CGame* game = new CGame(&createParameters);

    if (!engine->Init(&createParameters))
    {
        return EXIT_FAILURE;
    }
    return engine->Run();
}