#include "Engine.h"

//--------------------------------------------------------------------------------------
// Engine()
// Class constructor, set Engine specific values
//--------------------------------------------------------------------------------------
Engine::Engine(HINSTANCE instance, const std::wstring& className, const std::wstring& windowTitle, int showCommand)
    : m_Instance(instance), m_ClassName(className), m_WindowTitle(windowTitle), m_ShowCommand(showCommand)
{
    m_ScreenWidth = 1024;
    m_ScreenHeight = 768;

    //g_Graphics = new SDX::Graphics();
    //g_Input    = new SDX::Input(&m_Instance, &m_WindowHandle);
    
}

//--------------------------------------------------------------------------------------
// ~Engine()
// Class destructor, run Shutdown() and Exit()
//--------------------------------------------------------------------------------------
Engine::~Engine()
{
    Shutdown();
    Exit();
}

//--------------------------------------------------------------------------------------
// void Run()
// Initialise the application and run the main game loop
//--------------------------------------------------------------------------------------
void Engine::Run()
{
    InitWindow();
    g_Graphics.InitDirectX(&m_ScreenWidth, &m_ScreenHeight, &m_WindowHandle, &m_Window);
    g_Input.InitDirectInput(&m_Instance, &m_WindowHandle);
    MSG message;
    ZeroMemory(&message, sizeof(message));

    while (message.message != WM_QUIT)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            Update();
        }
    }

    // Game loop finished, shut everything down
    this->Exit();
}

//--------------------------------------------------------------------------------------
// void InitWindow()
// Register a new WINAPI window, create and show it
//--------------------------------------------------------------------------------------
void Engine::InitWindow()
{
    ZeroMemory(&m_Window, sizeof(m_Window));
    m_Window.cbSize        = sizeof(WNDCLASSEX);
    m_Window.style         = CS_CLASSDC;
    m_Window.lpfnWndProc   = WndProc;
    m_Window.hInstance     = m_Instance;
    m_Window.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    m_Window.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);
    m_Window.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    m_Window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    m_Window.lpszClassName = m_ClassName.c_str();

    RECT windowRectangle = { 0, 0, m_ScreenWidth, m_ScreenHeight };
    AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

    RegisterClassEx(&m_Window);
    m_WindowHandle = CreateWindow(m_ClassName.c_str(), m_WindowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, m_Instance, nullptr);

    ShowWindow(m_WindowHandle, m_ShowCommand);
    UpdateWindow(m_WindowHandle);
}

//--------------------------------------------------------------------------------------
// void Update()
// Take any input and make any updates before rendering to the screen
//--------------------------------------------------------------------------------------
void Engine::Update()
{
    g_Input.Process(&g_Graphics);
    g_Graphics.Render();
}

//--------------------------------------------------------------------------------------
// void Shutdown()
// Shutdown everything from the Engine class
//--------------------------------------------------------------------------------------
void Engine::Shutdown()
{
    
}

//--------------------------------------------------------------------------------------
// void Exit()
// Shutdown everything and exit
//--------------------------------------------------------------------------------------
void Engine::Exit()
{
    g_Graphics.Shutdown();
    g_Input.Shutdown();
    Shutdown();

    UnregisterClass(m_ClassName.c_str(), m_Window.hInstance);
}

//--------------------------------------------------------------------------------------
// LRESULT WINAPI WndProc()
// Windows procedure callback to deal with messages
//--------------------------------------------------------------------------------------
LRESULT WINAPI Engine::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    return DefWindowProc(windowHandle, message, wParam, lParam);
}