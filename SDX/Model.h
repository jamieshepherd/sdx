#include "shared.h"

#include <istream>
#include <fstream>

namespace SDX
{
    class Model
    {
        public:
            Model();
            virtual ~Model();

            bool LoadModel(char* filename);
            std::vector<VERTEX> GetVertices();
            std::vector<UINT>   GetIndices();
            void Release();

        protected:
            int       m_TotalVertices;

            ID3D11BlendState*     m_Transparency;
            ID3D11Buffer*         m_MeshVertexBuffer;
            ID3D11Buffer*         m_MeshIndexBuffer;
            XMMATRIX              m_MeshWorld;
            int                   meshSubsets = 0;
            std::vector<int>      meshSubsetIndexStart;
            std::vector<int>      meshSubsetTexture;

            std::vector<UINT>     indices;
            int                   triangleCount;
            std::vector<VERTEX>   vertexPositions;
            std::vector<XMFLOAT3> vertexNormal;
            std::vector<XMFLOAT2> vertexTextureCoord;
    };
}