#include "Input.h"

namespace SDX
{
    //--------------------------------------------------------------------------------------
    // Input()
    // Input constructor
    //--------------------------------------------------------------------------------------
    Input::Input()
    {
    }

    //--------------------------------------------------------------------------------------
    // Input()
    // Input destructor
    //--------------------------------------------------------------------------------------
    Input::~Input()
    {
    }

    //--------------------------------------------------------------------------------------
    // InitDirectInput()
    // Initialises DirectInput and gets keyboard and mouse devices
    //--------------------------------------------------------------------------------------
    void Input::InitDirectInput(HINSTANCE* m_Instance, HWND* m_WindowHandle)
    {
        ZeroMemory(m_KeyboardKeys, sizeof(m_KeyboardKeys));
        ZeroMemory(m_PrevKeyboardKeys, sizeof(m_PrevKeyboardKeys));

        // Initialise DirectInput8
        ThrowIfFailed(DirectInput8Create(*m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, 0), L"Couldn't initialise DirectInput");

        // Create a keyboard device
        ThrowIfFailed(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, 0), L"Couldn't create Direct Input KEYBOARD device");

        // Tell DirectInput what data format to read from the keyboard device
        ThrowIfFailed(m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard), L"Couldn't set data format for keyboard");

        // Set cooperative level for the keyboard device
        ThrowIfFailed(m_KeyboardDevice->SetCooperativeLevel(*m_WindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE), L"Couldn't set cooperative level for keyboard");

        // Acquire the keyboard device from the OS
        ThrowIfFailed(m_KeyboardDevice->Acquire(), L"Couldn't acquire the keyboard from the OS");

        // Create a mouse device
        ThrowIfFailed(m_DirectInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, 0), L"Couldn't create Direct Input MOUSE device");

        // Tell DirectInput what data format to read from the mouse device
        ThrowIfFailed(m_MouseDevice->SetDataFormat(&c_dfDIMouse), L"Couldn't set data format for mouse");

        // Set cooperative level for the mouse device
        ThrowIfFailed(m_MouseDevice->SetCooperativeLevel(*m_WindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE), L"Couldn't set cooperative level for mouse");

        // Acquire the mouse device from the OS
        ThrowIfFailed(m_MouseDevice->Acquire(), L"Couldn't acquire the mouse from the OS");
    }

    //--------------------------------------------------------------------------------------
    // void CheckState()
    // Check the state of our input devices
    //--------------------------------------------------------------------------------------
    void Input::CheckState()
    {
        HRESULT result;
        
        // Check the KEYBOARD device state
        result = m_KeyboardDevice->GetDeviceState(sizeof(m_KeyboardKeys), (LPVOID)&m_KeyboardKeys);

        // If we lost the keyboard device, try to get it back
        if (FAILED(result)) {
            result = m_KeyboardDevice->Acquire();
            while (result == DIERR_INPUTLOST) {
                result = m_KeyboardDevice->Acquire();
            }
        }

        result = m_MouseDevice->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);
    }

    //--------------------------------------------------------------------------------------
    // void Process()
    // Class destructor, run Shutdown() and Exit()
    //--------------------------------------------------------------------------------------
    void Input::Process(SDX::Graphics* g_Graphics, SDX::Camera* g_Camera)
    {
        CheckState();

        m_MouseWheel += m_MouseState.lZ;

        // If they press escape, post a quit message and exit
        if (GetAsyncKeyState(VK_ESCAPE)) {
            PostQuitMessage(0);
        }

        // Key UP
        if (KEYDOWN(m_KeyboardKeys, DIK_UP)) {
            if (g_Camera->m_CamMode == 2) {
                g_Graphics->MovePlayerForward();
            } else {
                g_Camera->MoveForward();
            }
        }

        // Key DOWN
        if (KEYDOWN(m_KeyboardKeys, DIK_DOWN)) {
            if (g_Camera->m_CamMode == 2) {
                g_Graphics->MovePlayerBackward();
            } else {
                g_Camera->MoveBackward();
            }
        }

        // Key LEFT
        if (KEYDOWN(m_KeyboardKeys, DIK_LEFT)) {
            if (g_Camera->m_CamMode == 2) {
                g_Graphics->MovePlayerLeft();
            } else {
                g_Camera->MoveLeft();
            }
        }

        // Key RIGHT
        if (KEYDOWN(m_KeyboardKeys, DIK_RIGHT)) {
            if (g_Camera->m_CamMode == 2) {
                g_Graphics->MovePlayerRight();
            } else {
                g_Camera->MoveRight();
            }
        }

        // F1 Key - Change topology
        if (KEYDOWN(m_KeyboardKeys, DIK_F1) && !KEYDOWN(m_PrevKeyboardKeys, DIK_F1)) {
            if (g_Graphics->rs_State == 1) {
                g_Graphics->SetRasterizer(0);
            } else {
                g_Graphics->SetRasterizer(1);
            }
        }

        // F2 Key - Change camera view
        if (KEYDOWN(m_KeyboardKeys, DIK_F2) && !KEYDOWN(m_PrevKeyboardKeys, DIK_F2)) {
            if (g_Camera->m_CamMode == 1) {
                g_Camera->SetCameraMode(2);
            } else {
                g_Camera->SetCameraMode(1);
            }
        }

        // Mouse XY
        if (m_PrevMouseState.lX != m_MouseState.lX || m_PrevMouseState.lY != m_MouseState.lY) {
            g_Camera->UpdateMouseXY(m_MouseState.lX, m_MouseState.lY);
        }

        // Copy the current set to previous keys for the next iteration
        memcpy(m_PrevKeyboardKeys, m_KeyboardKeys, sizeof(m_KeyboardKeys));
        memcpy(&m_PrevMouseState, &m_MouseState, sizeof(m_MouseState));
    }

    //--------------------------------------------------------------------------------------
    // void Shutdown()
    // Unacquire any devices and release objects
    //--------------------------------------------------------------------------------------
    void Input::Shutdown()
    {
        if (m_DirectInput) {
            if (m_KeyboardDevice) {
                m_KeyboardDevice->Unacquire();
                ReleaseObject(m_KeyboardDevice);
            }
            if (m_MouseDevice) {
                m_MouseDevice->Unacquire();
                ReleaseObject(m_MouseDevice);
            }
            ReleaseObject(m_DirectInput);
        }
    }
}