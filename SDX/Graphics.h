#pragma once

#include "Shared.h"

namespace SDX
{
    class Graphics
    {
        public:
            Graphics(UINT* m_pScreenWidth, UINT* m_pScreenHeight, HWND* m_pWindowHandle, WNDCLASSEX* m_pWindow);
            virtual ~Graphics();

            void InitDirectX();
            void CreateShaders();
            void LoadMesh();
            void Update();
            void Render();
            void Shutdown();

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

            ID3D11Buffer*           g_pViewConstantBuffer;
            ID3D11Buffer*           g_pProjectionConstantBuffer;
            ID3D11Buffer*           g_pWorldConstantBuffer;

            DirectX::XMMATRIX       m_WorldMatrix;
            DirectX::XMMATRIX       m_ViewMatrix;
            DirectX::XMMATRIX       m_ProjectionMatrix;
    };
}