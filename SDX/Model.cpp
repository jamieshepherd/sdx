#include "Model.h"

namespace SDX
{
    //--------------------------------------------------------------------------------------
    // Model()
    // Model constructor
    //--------------------------------------------------------------------------------------
    Model::Model(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext)
    {
        // Set the World matrix
        XMMATRIX t_World;
        t_World = XMLoadFloat4x4(&m_World);
        XMStoreFloat4x4(&m_World, t_World);

        g_pDevice = pDevice;
        g_pDeviceContext = pDeviceContext;
        m_Textured = false;
    }

    //--------------------------------------------------------------------------------------
    // Model()
    // Model destructor
    //--------------------------------------------------------------------------------------
    Model::~Model()
    {

    }

    //--------------------------------------------------------------------------------------
    // LoadModel()
    // Load a model from an .obj file
    //--------------------------------------------------------------------------------------
    bool Model::LoadModel(char* filename, wchar_t* texturename, bool leftHanded, bool isTextured)
    {
        m_Textured = isTextured;

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
                        m_VertexPositions.push_back(XMFLOAT3(vx, vy, vz));
                    } else {
                        m_VertexPositions.push_back(XMFLOAT3(vx, vy, vz*-1.0f));
                    }
                }
                // Vertex texture coordinates
                else if (streamChar == 't') {
                    float vtcu, vtcv;
                    fileStream >> std::skipws >> vtcu >> vtcv;

                    if (leftHanded) {
                        m_VertexTextureCoords.push_back(XMFLOAT2(vtcu, vtcv));
                    } else {
                        m_VertexTextureCoords.push_back(XMFLOAT2(vtcu, 1.0f - vtcv));
                    }
                    
                }
                // Vertex normal
                else if (streamChar == 'n') {
                    float vnx, vny, vnz;
                    fileStream >> vnx >> vny >> vnz;
                    m_VertexNormals.push_back(XMFLOAT3(vnx, vny, vnz));
                }
            }
            // Group
            else if (streamChar == 'g') {

            }
            // Index or face
            else if (streamChar == 'f') {
                UINT value = 0;
                m_TriangleCount = 0;
                
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

                m_TriangleCount++;
            }
            // Material library filename
            else if (streamChar == 'm') {

            }
            // Use material
            else if (streamChar == 'u') {

            }

            // S
            else if (streamChar == 's') {

            }
        }

        // Load vertices
        for (UINT i = 0; i < m_VertexPositions.size(); i++) {

            VERTEX newVertex;
            if (m_Textured) {
                newVertex = { m_VertexPositions[i], m_VertexTextureCoords[i] };
                //newVertex = { m_VertexPositions[i], XMFLOAT4(randColor(), randColor(), randColor(), 1.0f) };
            } else {
                newVertex = { m_VertexPositions[i], XMFLOAT2(0.0f, 0.0f) };
                //newVertex = { m_VertexPositions[i], XMFLOAT4(randColor(), randColor(), randColor(), 1.0f) };
            }
            
            m_Vertices.push_back(newVertex);
        }

        if (m_Textured) {
            LoadTexture(texturename);
        }

        LoadBuffers();

        return true;
    }

    //--------------------------------------------------------------------------------------
    // bool LoadTexture()
    // Load the texture file if there is one
    //--------------------------------------------------------------------------------------
    bool Model::LoadTexture(wchar_t* filename)
    {
        // Load texture file
        ThrowIfFailed(CreateWICTextureFromFile(g_pDevice, g_pDeviceContext, filename, nullptr, &m_MeshTexture), L"Couldn't load texture from file");

        // Create a texture sampler
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        ThrowIfFailed(g_pDevice->CreateSamplerState(&samplerDesc, &m_MeshTextureSamplerState), L"Couldn't create sampler state");

        return true;
    }

    //--------------------------------------------------------------------------------------
    // bool LoadBuffers()
    // Initialise and create our buffers()
    //--------------------------------------------------------------------------------------
    bool Model::LoadBuffers()
    {
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


        // Load index buffer
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(WORD) * m_Indices.size();
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        resourceData.pSysMem = &m_Indices[0];
        g_pDevice->CreateBuffer(&bufferDesc, &resourceData, &g_pIndexBuffer);

        return true;
    }

    //--------------------------------------------------------------------------------------
    // void DrawIndexed()
    // Draw our indices to the screen
    //--------------------------------------------------------------------------------------
    void Model::DrawIndexed()
    {
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        if (m_Textured) {
            g_pDeviceContext->PSSetShaderResources(0, 1, &m_MeshTexture);
            g_pDeviceContext->PSSetSamplers(0, 1, &m_MeshTextureSamplerState);
        }        

        // Set vertex buffer to this one
        g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
        // Set index buffer to this one
        g_pDeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        g_pDeviceContext->DrawIndexed(m_Indices.size(), 0, 0);
    }
    
    //--------------------------------------------------------------------------------------
    // XMMATRIX GetWorld()
    // Get the World matrix for this model
    //--------------------------------------------------------------------------------------
    XMMATRIX Model::GetWorld()
    {
        XMMATRIX t_World;
        t_World = XMLoadFloat4x4(&m_World);
        return t_World;
    }

    //--------------------------------------------------------------------------------------
    // void Shutdown()
    // Release objects
    //--------------------------------------------------------------------------------------
    void Model::Shutdown()
    {
        ReleaseObject(g_pVertexBuffer);
        ReleaseObject(g_pIndexBuffer);
    }
}
