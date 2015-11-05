#pragma once

#include "Shared.h"
#include "DDSTextureLoader.h"

namespace SDX
{
    class Skybox
    {
        public:
            Skybox(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext);
            virtual ~Skybox();

            void CreateSphere(int LatLines, int LongLines);
            void LoadShaders();
            void Render();
        protected:
            ID3D11Device1*        g_pDevice;
            ID3D11DeviceContext1* g_pDeviceContext;

            ID3D11Buffer* sphereIndexBuffer;
            ID3D11Buffer* sphereVertBuffer;

            ID3D11VertexShader* SKYMAP_VS;
            ID3D11PixelShader* SKYMAP_PS;
            ID3D10Blob* SKYMAP_VS_Buffer;
            ID3D10Blob* SKYMAP_PS_Buffer;

            ID3D11ShaderResourceView* smrv;

            ID3D11DepthStencilState* DSLessEqual;
            ID3D11RasterizerState* RSCullNone;

            int NumSphereVertices;
            int NumSphereFaces;

            XMMATRIX Rotationx;
            XMMATRIX Rotationy;
            XMMATRIX Rotationz;

            XMMATRIX sphereWorld;

    };
}