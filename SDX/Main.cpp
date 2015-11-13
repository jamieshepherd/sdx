#include "Engine.h"

//--------------------------------------------------------------------------------------
// int WINAPI WinMain()
// Main entry point for our program. Initialise the game object and Run() it.
//--------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
    std::unique_ptr<Engine> game(new Engine(instance, L"SDXEngine", L"Shepherd DirectX", showCommand));

    game->Run();

    return 0;
}