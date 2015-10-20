#include "Engine.h"

//--------------------------------------------------------------------------------------
// int WINAPI WinMain()
// Main entry point for our program. Initialise the game object and Run() it.
//--------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
    auto game = std::make_unique<Engine>(instance, L"SDXEngine", L"Shepherd DirectX", showCommand);
    //std::unique_ptr<Engine> game(new Engine(instance, L"SDXEngine", L"Shepherd DirectX", showCommand));

    game->Run();

    /*try {
        game->Run();
    } catch (GameException ex) {
        MessageBox(game->WindowHandle(), ex.whatw().c_str(), game->WindowTitle().c_str(), MB_ABORTRETRYIGNORE);
    }*/

    return 0;
}