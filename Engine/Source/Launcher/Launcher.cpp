#include "Engine.h"
#include "Game.h"
#include "CreateParameters.h"

int main() 
{
    SCreateParameters createParameters;

    CGame* game = new CGame(&createParameters);

    if (!CEngine::Start(&createParameters))
    {
        return EXIT_FAILURE;
    }
    return CEngine::Run();
}