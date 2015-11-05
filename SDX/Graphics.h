#pragma once

#include "Shared.h"
#include "Camera.h"
#include "Model.h"
#include "Terrain.h"
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
            void LoadSkybox();
            void LoadRasterizers();
            void LoadMesh();
            void Update(SDX::Camera* g_Camera);
            void LoadTerrain();
            void SetRasterizer(int state);
            XMMATRIX GetWVP(XMFLOAT4X4* world);
            void Render(SDX::Camera* g_Camera);

            void MovePlayerForward();
            void MovePlayerBackward();
            void MovePlayerLeft();
            void MovePlayerRight();

            void Shutdown();

            int                     rs_State = 1;
            int                     m_CamMode = 1;
            ID3D11RasterizerState*  rs_Solid;
            ID3D11RasterizerState*  rs_WireFrame;

            Terrain*                g_Terrain;
            Skybox*                 g_Skybox;
            Model*                  g_Model1;
            Model*                  g_Model2;

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
            ID3D11SamplerState*       m_FontTextureSamplerState;
            ID3D11SamplerState*       m_MeshTextureSamplerState;

            ID3D11VertexShader*       m_pVertexShader;
            ID3D11PixelShader*        m_pPixelShader;
            ID3D11PixelShader*        m_pPixelShaderNT;

            ID3D11InputLayout*        m_pInputLayout;

            ID3D11Buffer*             g_pFontVertexBuffer;
            ID3D11Buffer*             g_pVertexBuffer;
            ID3D11Buffer*             g_pIndexBuffer;

            ID3D11Buffer*             cbPerObjectBuffer;
            ID3D11Buffer*             cbPerObjectManipBuffer;

            XMFLOAT4X4                m_WVP;

            XMFLOAT4X4                m_GroundWorld;
            XMFLOAT4X4                m_SphereWorld;
            XMFLOAT4X4                m_Object1World;
            XMFLOAT4X4                m_Object2World;

            XMFLOAT4X4                m_CamView;
            XMFLOAT4X4                m_CamProjection;

            XMVECTOR                  m_CamPosition;
            XMVECTOR                  m_CamTarget;
            XMVECTOR                  m_CamUp;
    };
}