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

            void CreateSphere();
            void LoadShaders();
            void Render();
        protected:
            ID3D11Device1*        g_pDevice;
            ID3D11DeviceContext1* g_pDeviceContext;
    };
}