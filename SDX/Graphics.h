#pragma once

#include "Shared.h"
#include "Camera.h"
#include "Model.h"

namespace SDX
{
    class Graphics
    {
        public:
            Graphics();
            virtual ~Graphics();

            void InitDirectX(UINT* m_pScreenWidth, UINT* m_pScreenHeight, HWND* m_pWindowHandle, WNDCLASSEX* m_pWindow);
            void LoadShaders();
            void LoadRasterizers();
            void LoadMesh();
            void Update(SDX::Camera* g_Camera);
            void SetRasterizer(int state);
            void LoadFont();
            void DrawString(char* text, float positionX, float positionY);
            void Render();
            void Shutdown();

            int                     rs_State = 1;
            ID3D11RasterizerState*  rs_Solid;
            ID3D11RasterizerState*  rs_WireFrame;

        protected:
            UINT*                   m_ScreenWidth;
            UINT*                   m_ScreenHeight;
            HWND*                   m_WindowHandle;
            WNDCLASSEX*             m_Window;

            D3D_DRIVER_TYPE         m_DriverType;
            D3D_FEATURE_LEVEL       m_FeatureLevel;
            ID3D11Device1*          m_pDirect3DDevice;
            ID3D11DeviceContext1*   m_pDirect3DDeviceContext;
            IDXGISwapChain1*        m_pSwapChain;
            ID3D11RenderTargetView* m_pRenderTargetView;

            ID3D11Texture2D*        m_pDepthStencilBuffer;
            ID3D11DepthStencilView* m_pDepthStencilView;            

            ID3D11VertexShader*     m_pVertexShader;
            ID3D11PixelShader*      m_pPixelShader;

            ID3D11InputLayout*      m_pInputLayout;

            ID3D11Buffer*           g_pVertexBuffer;
            ID3D11Buffer*           g_pIndexBuffer;

            ID3D11Buffer*           g_pConstantBuffer;
            ID3D11Buffer*           g_pWorldConstantBuffer;
            ID3D11Buffer*           g_pViewConstantBuffer;
            ID3D11Buffer*           g_pProjectionConstantBuffer;

            XMFLOAT4X4              m_WorldMatrix;
            XMFLOAT4X4              m_ViewMatrix;
            XMFLOAT4X4              m_ProjectionMatrix;
    };
}