#pragma once

#include "Shared.h"
#include "Input.h"
#include "Graphics.h"
#include "Camera.h"

using namespace SDX;

namespace SDX {
    class Engine {
        public:
            Engine(HINSTANCE instance, const std::wstring& className, const std::wstring& windowTitle, int showCommand);
            virtual ~Engine();

            SDX::Camera             g_Camera;
            SDX::Graphics           g_Graphics;
            SDX::Input              g_Input;

            void Run();
            void InitWindow();
            
            void Update();
            void Shutdown();
            void Exit();

        protected:
            HINSTANCE               m_Instance;
            std::wstring            m_ClassName;
            std::wstring            m_WindowTitle;
            int                     m_ShowCommand;

            UINT                    m_ScreenWidth;
            UINT                    m_ScreenHeight;
            HWND                    m_WindowHandle;
            WNDCLASSEX              m_Window;            

        private:
            static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
    };
}