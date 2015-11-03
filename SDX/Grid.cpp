#include "Grid.h"

namespace SDX
{
    Grid::Grid(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext) : g_pDevice(pDevice), g_pDeviceContext(pDeviceContext)
    {
        m_VertexBuffer = 0;
        m_IndexBuffer = 0;
        m_TerrainWidth = 100;
        m_TerrainHeight = 100;
    }

    Grid::~Grid()
    {
    }

    void Grid::Init()
    {
        GridVERTEX* vertices;
        unsigned long* indices;
        int index, i, j;
        float positionX, positionZ;
        D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
        D3D11_SUBRESOURCE_DATA vertexData, indexData;

        // Calculate the number of vertices in the terrain mesh.
        m_VertexCount = (m_TerrainWidth - 1) * (m_TerrainHeight - 1) * 8;

        // Set the index count to the same as the vertex count.
        m_IndexCount = m_VertexCount;

        // Create the vertex array.
        vertices = new GridVERTEX[m_VertexCount];

        // Create the index array.
        indices = new unsigned long[m_IndexCount];

        // Initialize the index to the vertex array.
        index = 0;

        // Load the vertex and index arrays with the terrain data.
        for (j = 0; j<(m_TerrainHeight - 1); j++)
        {
            for (i = 0; i<(m_TerrainWidth - 1); i++)
            {
                // LINE 1
                // Upper left.
                positionX = (float)i;
                positionZ = (float)(j + 1);

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Upper right.
                positionX = (float)(i + 1);
                positionZ = (float)(j + 1);

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // LINE 2
                // Upper right.
                positionX = (float)(i + 1);
                positionZ = (float)(j + 1);

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Bottom right.
                positionX = (float)(i + 1);
                positionZ = (float)j;

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // LINE 3
                // Bottom right.
                positionX = (float)(i + 1);
                positionZ = (float)j;

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Bottom left.
                positionX = (float)i;
                positionZ = (float)j;

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // LINE 4
                // Bottom left.
                positionX = (float)i;
                positionZ = (float)j;

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Upper left.
                positionX = (float)i;
                positionZ = (float)(j + 1);

                vertices[index].pos = XMFLOAT3(positionX, 0.0f, positionZ);
                vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;
            }
        }

        // Set up the description of the static vertex buffer.
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(GridVERTEX) * m_VertexCount;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the vertex data.
        vertexData.pSysMem = vertices;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        // Now create the vertex buffer.
        g_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
        
        // Set up the description of the static index buffer.
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the index data.
        indexData.pSysMem = indices;
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        // Create the index buffer.
        g_pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);

        // Release the arrays now that the buffers have been created and loaded.
        delete[] vertices;
        vertices = 0;

        delete[] indices;
        indices = 0;

        LoadShaders();
    }

    void Grid::LoadShaders()
    {
        // Compile our shaders to a blob type
        ID3DBlob* pVSBlob = nullptr;
        ID3DBlob* pPSBlob = nullptr;
        D3DCompileFromFile(L"GridVertexShader.hlsl", nullptr, nullptr, "main", "vs_4_0", 0, 0, &pVSBlob, 0);
        D3DCompileFromFile(L"GridPixelShader.hlsl", nullptr, nullptr, "main", "ps_4_0", 0, 0, &pPSBlob, 0);

        // Create our shaders
        g_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
        g_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
        
        // Describe how the input will be laid out to the buffer
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        UINT totalLayoutElements = ARRAYSIZE(inputElementDesc);

        g_pDevice->CreateInputLayout(inputElementDesc, totalLayoutElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout);
        

        ReleaseObject(pVSBlob);
        ReleaseObject(pPSBlob);
    }

    void Grid::Render()
    {

        unsigned int stride;
        unsigned int offset;

        // Set vertex buffer stride and offset.
        stride = sizeof(GridVERTEX);
        offset = 0;

        g_pDeviceContext->IASetInputLayout(m_pInputLayout);

        // Set shaders to grid shader
        g_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        g_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

        // Set the vertex buffer to active in the input assembler so it can be rendered.
        g_pDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

        // Set the index buffer to active in the input assembler so it can be rendered.
        g_pDeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
        g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

        // Draw the grid
        g_pDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
    }

    void Grid::Shutdown()
    {
    }
}