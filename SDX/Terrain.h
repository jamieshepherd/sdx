#pragma once

#include "Shared.h"

namespace SDX
{
    class Terrain
    {
    public:
        Terrain(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext);
        virtual ~Terrain();
        void Init();
        void Render();
        void Shutdown();
    protected:
        ID3D11Device1*            g_pDevice;
        ID3D11DeviceContext1*     g_pDeviceContext;

        ID3D11Buffer*             m_VertexBuffer;
        ID3D11Buffer*             m_IndexBuffer;

        ID3D11ShaderResourceView* m_CubesTexture;
        ID3D11SamplerState*       m_CubesTexSamplerState;
    };
}