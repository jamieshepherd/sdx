#pragma once

#include <istream>
#include <fstream>
#include "shared.h"

namespace SDX
{
    class Model
    {
        public:
            Model(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext);
            virtual ~Model();

            bool                      LoadModel(char* filename, wchar_t* texturename, bool leftHanded, bool isTextured);
            bool                      LoadTexture(wchar_t* filename);
            bool                      LoadBuffers();
            void                      DrawIndexed();
            XMMATRIX                  GetWorld();
            void                      Release();

            ID3D11Device1*            g_pDevice;
            ID3D11DeviceContext1*     g_pDeviceContext;
            ID3D11Buffer*             g_pVertexBuffer;
            ID3D11Buffer*             g_pIndexBuffer;

        protected:
            ID3D11BlendState*         m_Transparency;
            ID3D11Buffer*             m_MeshVertexBuffer;
            ID3D11Buffer*             m_MeshIndexBuffer;

            XMFLOAT4X4                m_World;

            ID3D11ShaderResourceView* m_MeshTexture;
            ID3D11SamplerState*       m_MeshTextureSamplerState;

            std::vector<WORD>         m_Indices;
            std::vector<VERTEX>       m_Vertices;
            int                       m_TriangleCount;  

            std::vector<XMFLOAT3>     m_VertexPositions;
            std::vector<XMFLOAT3>     m_VertexNormals;
            std::vector<XMFLOAT2>     m_VertexTextureCoords;

            bool                      m_Textured;
    };
}