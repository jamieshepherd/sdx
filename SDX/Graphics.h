#pragma once

#include "Shared.h"
#include "Camera.h"
#include "Grid.h"
#include "Model.h"
#include "Skybox.h"

namespace SDX
{
    class Graphics
    {
        public:
            Graphics();
            virtual ~Graphics();

            void InitDirectX(UINT* m_pScreenWidth, UINT* m_pScreenHeight, HWND* m_pWindowHandle, WNDCLASSEX* m_pWindow);
            void LoadShaders();
            void LoadGrid();
            void LoadSkybox();
            void LoadRasterizers();
            void LoadMesh();
            void Update(SDX::Camera* g_Camera);
            void SetRasterizer(int state);
            void LoadFont();
            void DrawString(char* text, float positionX, float positionY);
            XMMATRIX GetWVP(XMFLOAT4X4 world);
            void Render();
            void Shutdown();

            int                     rs_State = 1;
            ID3D11RasterizerState*  rs_Solid;
            ID3D11RasterizerState*  rs_WireFrame;

            Grid*                   g_Grid;
            Model*                  g_Model1;
            Model*                  g_Model2;
            Skybox*                 g_Skybox;

            XMMATRIX                g_Rotation;
            XMMATRIX                g_Scale;
            XMMATRIX                g_Translation;

        protected:
            UINT*                     m_ScreenWidth;
            UINT*                     m_ScreenHeight;
            HWND*                     m_WindowHandle;
            WNDCLASSEX*               m_Window;

            D3D_DRIVER_TYPE           m_DriverType;
            D3D_FEATURE_LEVEL         m_FeatureLevel;
            ID3D11Device1*            m_pDirect3DDevice;
            ID3D11DeviceContext1*     m_pDirect3DDeviceContext;
            IDXGISwapChain1*          m_pSwapChain;
            ID3D11RenderTargetView*   m_pRenderTargetView;

            ID3D11Texture2D*          m_pDepthStencilBuffer;
            ID3D11DepthStencilView*   m_pDepthStencilView;  

            ID3D11ShaderResourceView* m_MeshTexture;
            ID3D11ShaderResourceView* m_FontTexture;
            ID3D11SamplerState*       m_MeshTextureSamplerState;

            ID3D11VertexShader*       m_pVertexShader;
            ID3D11PixelShader*        m_pPixelShader;

            ID3D11InputLayout*        m_pInputLayout;

            ID3D11Buffer*             g_pVertexBuffer;
            ID3D11Buffer*             g_pIndexBuffer;

            ID3D11Buffer*             cbPerObjectBuffer;

            XMFLOAT4X4                m_WVP;
            //XMFLOAT4X4                m_World;

            XMFLOAT4X4                m_GridWorld;
            XMFLOAT4X4                m_Object1World;
            XMFLOAT4X4                m_Object2World;

            XMFLOAT4X4                m_CamView;
            XMFLOAT4X4                m_CamProjection;

            XMVECTOR                  m_CamPosition;
            XMVECTOR                  m_CamTarget;
            XMVECTOR                  m_CamUp;
    };
}