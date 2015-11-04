#include "Terrain.h"

namespace SDX
{
    Terrain::Terrain(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext)
    {
        g_pDevice = pDevice;
        g_pDeviceContext = pDeviceContext;
    }

    Terrain::~Terrain()
    {

    }

    void Terrain::Init()
    {
        //Create the vertex buffer
        VERTEX vertices[] =
        {
            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(100.0f, 100.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 100.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(100.0f, 0.0f) }
        };

        WORD indices[] = {
            0, 1, 2,
            0, 2, 3,
        };

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(WORD) * 2 * 3;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA iinitData;

        iinitData.pSysMem = indices;
        g_pDevice->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(VERTEX) * 4;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;

        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = vertices;
        g_pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

        // Assign texture
        ThrowIfFailed(CreateWICTextureFromFile(g_pDevice, g_pDeviceContext, L"Textures/grass.jpg", nullptr, &CubesTexture), L"Couldn't load texture from file");

        // Describe the Sample State
        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory(&sampDesc, sizeof(sampDesc));
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        //Create the Sample State
        g_pDevice->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
    }

    void Terrain::Render()
    {
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        g_pDeviceContext->PSSetShaderResources(0, 1, &CubesTexture);
        g_pDeviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);

        // Set index buffer to this one
        g_pDeviceContext->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // Set vertex buffer to this one
        g_pDeviceContext->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);
        
        g_pDeviceContext->DrawIndexed(6, 0, 0);
    }
}