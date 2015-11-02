#pragma once

#include "shared.h"
#include <istream>
#include <fstream>

namespace SDX
{
    class Model
    {
        public:
            Model(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext);
            virtual ~Model();

            bool                  LoadModel(char* filename, bool leftHanded, bool isTextured);
            bool                  LoadBuffers();
            void                  DrawIndexed();
            std::vector<VERTEX>   GetVertices();
            std::vector<WORD>     GetIndices();
            XMMATRIX              GetWorld();
            void                  Release();

            ID3D11Device1*        g_pDevice;
            ID3D11DeviceContext1* g_pDeviceContext;

            ID3D11Buffer*         g_pVertexBuffer;
            ID3D11Buffer*         g_pIndexBuffer;

        protected:
            int                   m_TotalVertices;

            ID3D11BlendState*     m_Transparency;
            ID3D11Buffer*         m_MeshVertexBuffer;
            ID3D11Buffer*         m_MeshIndexBuffer;

            XMFLOAT4X4            m_World;

            int                   meshSubsets = 0;
            std::vector<int>      meshSubsetIndexStart;
            std::vector<int>      meshSubsetTexture;

            std::vector<WORD>     m_Indices;
            std::vector<VERTEX>   m_Vertices;
            int                   triangleCount;  

            std::vector<XMFLOAT3> vertexPositions;
            std::vector<XMFLOAT3> vertexNormals;
            std::vector<XMFLOAT2> vertexTextureCoords;
    };
}