#pragma once

#include "Shared.h"
#include "Graphics.h"

namespace SDX
{
    class Input
    {
        public:
            Input();
            virtual ~Input();

            void InitDirectInput(HINSTANCE* m_Instance, HWND* m_WindowHandle);
            void CheckState();
            void Process(SDX::Graphics* g_Graphics, SDX::Camera* g_Camera);
            void Shutdown();

        protected:
            HINSTANCE*           m_Instance;
            HWND*                m_WindowHandle;
            LPDIRECTINPUT8       m_DirectInput;
            LPDIRECTINPUTDEVICE8 m_KeyboardDevice;
            LPDIRECTINPUTDEVICE8 m_MouseDevice;
            char                 m_KeyboardKeys[256];
            char                 m_PrevKeyboardKeys[256];
            DIMOUSESTATE         m_MouseState;
            DIMOUSESTATE         m_PrevMouseState;
            long                 m_MouseX;
            long                 m_MouseY;
            long                 m_MouseWheel;
    };
}