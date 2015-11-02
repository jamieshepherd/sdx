#include "Model.h"

namespace SDX
{
    float randColor()
    {
        float randomColor = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        return randomColor;
    }

    Model::Model(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext)
    {
        // Set the World matrix
        XMMATRIX t_World;
        t_World = XMLoadFloat4x4(&m_World);
        t_World = XMMatrixIdentity();
        XMStoreFloat4x4(&m_World, t_World);

        g_pDevice = pDevice;
        g_pDeviceContext = pDeviceContext;
    }

    Model::~Model()
    {

    }

    bool Model::LoadModel(char* filename, bool leftHanded, bool isTextured)
    {
        std::ifstream fileStream;
        char streamChar;

        fileStream.open(filename);
        while (!fileStream.eof()) {
            // READ LINES
            fileStream.get(streamChar);
            // The line is a comment
            if (streamChar == '#') {
                while (streamChar != '\n') {
                    fileStream.get(streamChar);
                }
            }
            // Could be 'v' or 'vt' or 'vn'
            else if (streamChar == 'v') {
                fileStream.get(streamChar);
                // Vertex position
                if (streamChar == ' ') {
                    float vx, vy, vz;
                    fileStream >> std::skipws >> vx >> vy >> vz;
                    
                    if (leftHanded) {
                        vertexPositions.push_back(XMFLOAT3(vx, vy, vz));
                    } else {
                        vertexPositions.push_back(XMFLOAT3(vx, vy, vz*-1.0f));
                    }
                }
                // Vertex texture coordinates
                else if (streamChar == 't') {
                    float vtcu, vtcv;
                    fileStream >> vtcu >> vtcv;

                    if (leftHanded) {
                        vertexTextureCoords.push_back(XMFLOAT2(vtcu, vtcv));
                    } else {
                        vertexTextureCoords.push_back(XMFLOAT2(vtcu, 1.0f - vtcv));
                    }
                    
                }
                // Vertex normal
                else if (streamChar == 'n') {
                    float vnx, vny, vnz;
                    fileStream >> vnx >> vny >> vnz;
                    vertexNormals.push_back(XMFLOAT3(vnx, vny, vnz));
                }
            }
            // Group
            else if (streamChar == 'g') {

            }
            // Index or face
            else if (streamChar == 'f') {
                WORD value = 0;
                std::wstring VertexDefinition;
                triangleCount = 0;

                fileStream.ignore(); // Get rid of the first space

                // As long as we're on the same line
                while (streamChar != '\n') {

                    // TAKE THE FIRST CHAR, THIS IS AN INDEX
                    fileStream >> std::skipws >> value;
                    fileStream.get(streamChar);

                    while (streamChar != ' ') {
                        fileStream.get(streamChar);
                        if (streamChar == '\n') {
                            break;
                        }
                    }

                    m_Indices.push_back((UINT)value - 1);
                }

                triangleCount++;
            }
            // Material library filename
            else if (streamChar == 'm') {

            }
            // Use material
            else if (streamChar == 'u') {

            }
        }

        // Load vertices
        for (UINT i = 0; i < vertexPositions.size(); i++) {

            VERTEX newVertex;
            if (isTextured) {
                //newVertex = { vertexPositions[i], vertexTextureCoords[i] };
                newVertex = { vertexPositions[i], XMFLOAT4(randColor(), randColor(), randColor(), 1.0f) };
            } else {
                //newVertex = { vertexPositions[i], XMFLOAT2(0.0f, 0.0f) };
                newVertex = { vertexPositions[i], XMFLOAT4(randColor(), randColor(), randColor(), 1.0f) };
            }
            
            m_Vertices.push_back(newVertex);
        }

        LoadBuffers();

        return true;
    }

    bool Model::LoadBuffers()
    {
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        // Load vertex buffer
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;             // Write access for CPU and GPU
        bufferDesc.ByteWidth = sizeof(VERTEX) * m_Vertices.size();          // Size is the vertex struct times the amount of vertices
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // Use as a vertex buffer

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = &m_Vertices[0];

        g_pDevice->CreateBuffer(&bufferDesc, &resourceData, &g_pVertexBuffer);

        // Set vertex buffer to this one
        g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

        // Load index buffer
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(WORD) * m_Indices.size();
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        resourceData.pSysMem = &m_Indices[0];
        g_pDevice->CreateBuffer(&bufferDesc, &resourceData, &g_pIndexBuffer);

        // Set index buffer to this one
        g_pDeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        return true;
    }

    void Model::DrawIndexed()
    {
        g_pDeviceContext->DrawIndexed(m_Indices.size(), 0, 0);
    }

    std::vector<VERTEX> Model::GetVertices()
    {
        return m_Vertices;
    }

    std::vector<WORD> Model::GetIndices()
    {
        return m_Indices;
    }

    XMMATRIX Model::GetWorld()
    {
        XMMATRIX t_World;
        t_World = XMLoadFloat4x4(&m_World);
        return t_World;
    }

    void Model::Release()
    {
        ReleaseObject(m_MeshVertexBuffer);
        ReleaseObject(m_MeshIndexBuffer);
        ReleaseObject(m_Transparency);
    }
}
