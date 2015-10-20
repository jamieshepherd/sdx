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

        // ERROR CHECK
        DirectInput8Create(*m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, 0);

        // ERROR CHECK
        m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, 0);


        // ERROR CHECK
        // Tell DirectInput what data format to read from the device
        m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);

        // ERROR CHECK
        // Tell the system how the device will work with the system
        m_KeyboardDevice->SetCooperativeLevel(*m_WindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

        // ERROR CHECK
        // Acquire the device from the OS
        m_KeyboardDevice->Acquire();

    }

    //--------------------------------------------------------------------------------------
    // void Process()
    // Class destructor, run Shutdown() and Exit()
    //--------------------------------------------------------------------------------------
    void Input::Process()
    {
        // Get the current state of the keyboard device
        m_KeyboardDevice->GetDeviceState(sizeof(m_KeyboardKeys), (LPVOID)&m_KeyboardKeys);

        if (GetAsyncKeyState(VK_ESCAPE)) {
            PostQuitMessage(0);
        }

        // Down arrow KEY UP (key was down, now it's not, therefore key up)
        if (KEYDOWN(m_PrevKeyboardKeys, DIK_DOWN) && !KEYDOWN(m_KeyboardKeys, DIK_DOWN)) {
            // Do something
        }

        // Up arrow KEY UP (key was down, now it's not, therefore key up)
        if (KEYDOWN(m_PrevKeyboardKeys, DIK_UP) && !KEYDOWN(m_KeyboardKeys, DIK_UP)) {
            // Do something
            PostQuitMessage(0);
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