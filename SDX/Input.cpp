#include "Input.h"

namespace SDX
{
    Input::Input()
    {
    }

    Input::~Input()
    {
    }

    void Input::InitDirectInput(HINSTANCE* m_Instance, HWND* m_WindowHandle)
    {
        ZeroMemory(m_KeyboardKeys, sizeof(m_KeyboardKeys));
        ZeroMemory(m_PrevKeyboardKeys, sizeof(m_PrevKeyboardKeys));

        // Initialise DirectInput8
        ThrowIfFailed(DirectInput8Create(*m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, 0), L"Couldn't initialise DirectInput");

        // Create a keyboard device
        ThrowIfFailed(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, 0), L"Couldn't create Direct Input device");

        // Tell DirectInput what data format to read from the device
        ThrowIfFailed(m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard), L"Couldn't set data format to keyboard");

        // Tell the system how the device will work with the system
        ThrowIfFailed(m_KeyboardDevice->SetCooperativeLevel(*m_WindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE), L"Couldn't set cooperative level");

        // Acquire the device from the OS
        ThrowIfFailed(m_KeyboardDevice->Acquire(), L"Couldn't acquire the keyboard from the OS");

    }

    //--------------------------------------------------------------------------------------
    // void Process()
    // Class destructor, run Shutdown() and Exit()
    //--------------------------------------------------------------------------------------
    void Input::Process(SDX::Graphics* g_Graphics, SDX::Camera* g_Camera)
    {
        // Get the current state of the keyboard device
        m_KeyboardDevice->GetDeviceState(sizeof(m_KeyboardKeys), (LPVOID)&m_KeyboardKeys);

        if (GetAsyncKeyState(VK_ESCAPE)) {
            PostQuitMessage(0);
        }

        // Key UP
        if (KEYDOWN(m_KeyboardKeys, DIK_UP)) {
            g_Camera->MoveForward();
        }

        // Key DOWN
        if (KEYDOWN(m_KeyboardKeys, DIK_DOWN)) {
            g_Camera->MoveBackward();
        }

        // Key LEFT
        if (KEYDOWN(m_KeyboardKeys, DIK_LEFT)) {
            g_Camera->MoveLeft();
        }

        // Key RIGHT
        if (KEYDOWN(m_KeyboardKeys, DIK_RIGHT)) {
            g_Camera->MoveRight();
        }

        // F1 Key - Change topology
        if (KEYDOWN(m_KeyboardKeys, DIK_F1) && !KEYDOWN(m_PrevKeyboardKeys, DIK_F1)) {
            if (g_Graphics->g_Topology != D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP) {
                g_Graphics->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
            }
            else {
                g_Graphics->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            }
        }

        // Copy the current set to previous keys for the next iteration
        memcpy(m_PrevKeyboardKeys, m_KeyboardKeys, sizeof(m_KeyboardKeys));
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
            ReleaseObject(m_DirectInput);
        }
    }
}