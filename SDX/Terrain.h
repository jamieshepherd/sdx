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
    protected:
        ID3D11Device1*        g_pDevice;
        ID3D11DeviceContext1* g_pDeviceContext;

        ID3D11Buffer* squareVertBuffer;
        ID3D11Buffer* squareIndexBuffer;

        ID3D11ShaderResourceView* CubesTexture;
        ID3D11SamplerState*       CubesTexSamplerState;
    };
}