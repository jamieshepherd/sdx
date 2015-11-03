#pragma once

#include "Shared.h"

namespace SDX
{
    class Grid
    {
        public:
            Grid(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext);
            virtual ~Grid();

            void Init();
            void LoadShaders();
            void Render();
            void Shutdown();
        protected:
            ID3D11Device1*        g_pDevice;
            ID3D11DeviceContext1* g_pDeviceContext;
            ID3D11VertexShader*   m_pVertexShader;
            ID3D11PixelShader*    m_pPixelShader;
            ID3D11InputLayout*    m_pInputLayout;
            ID3D11Buffer*         m_VertexBuffer;
            ID3D11Buffer*         m_IndexBuffer;
            int                   m_TerrainWidth;
            int                   m_TerrainHeight;
            int                   m_VertexCount;
            int                   m_IndexCount;
    private:
        struct GridVERTEX
        {
            XMFLOAT3 pos;
            XMFLOAT4 color;
        };
    };
}