#pragma once

#include "Shared.h"

namespace SDX
{
    class Input
    {
        public:
            Input();
            virtual ~Input();

            void InitDirectInput(HINSTANCE* m_Instance, HWND* m_WindowHandle);
            void Process();
            void Shutdown();

        protected:
            HINSTANCE*           m_Instance;
            HWND*                m_WindowHandle;
            LPDIRECTINPUT8       m_DirectInput;
            LPDIRECTINPUTDEVICE8 m_KeyboardDevice;
            char                 m_KeyboardKeys[256];
            char                 m_PrevKeyboardKeys[256];
    };
}