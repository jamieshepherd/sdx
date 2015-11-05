#include "Skybox.h"

namespace SDX
{
    Skybox::Skybox(ID3D11Device1* pDevice, ID3D11DeviceContext1* pDeviceContext)
    {
        g_pDevice = pDevice;
        g_pDeviceContext = pDeviceContext;
    }

    Skybox::~Skybox()
    {
    }

    void Skybox::CreateSphere(int LatLines, int LongLines)
    {
        NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
        NumSphereFaces = ((LatLines - 3)*(LongLines)* 2) + (LongLines * 2);

        float sphereYaw = 0.0f;
        float spherePitch = 0.0f;

        std::vector<VERTEX> vertices(NumSphereVertices);

        XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

        vertices[0].pos.x = 0.0f;
        vertices[0].pos.y = 0.0f;
        vertices[0].pos.z = 1.0f;

        for (DWORD i = 0; i < LatLines - 2; ++i)
        {
            spherePitch = (i + 1) * (3.14 / (LatLines - 1));
            Rotationx = XMMatrixRotationX(spherePitch);
            for (DWORD j = 0; j < LongLines; ++j)
            {
                sphereYaw = j * (6.28 / (LongLines));
                Rotationy = XMMatrixRotationZ(sphereYaw);
                currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
                currVertPos = XMVector3Normalize(currVertPos);
                vertices[i*LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
                vertices[i*LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
                vertices[i*LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
            }
        }

        vertices[NumSphereVertices - 1].pos.x = 0.0f;
        vertices[NumSphereVertices - 1].pos.y = 0.0f;
        vertices[NumSphereVertices - 1].pos.z = -1.0f;


        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(VERTEX) * NumSphereVertices;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;

        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = &vertices[0];
        g_pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


        std::vector<DWORD> indices(NumSphereFaces * 3);

        int k = 0;
        for (DWORD l = 0; l < LongLines - 1; ++l)
        {
            indices[k] = 0;
            indices[k + 1] = l + 1;
            indices[k + 2] = l + 2;
            k += 3;
        }

        indices[k] = 0;
        indices[k + 1] = LongLines;
        indices[k + 2] = 1;
        k += 3;

        for (DWORD i = 0; i < LatLines - 3; ++i)
        {
            for (DWORD j = 0; j < LongLines - 1; ++j)
            {
                indices[k] = i*LongLines + j + 1;
                indices[k + 1] = i*LongLines + j + 2;
                indices[k + 2] = (i + 1)*LongLines + j + 1;

                indices[k + 3] = (i + 1)*LongLines + j + 1;
                indices[k + 4] = i*LongLines + j + 2;
                indices[k + 5] = (i + 1)*LongLines + j + 2;

                k += 6; // next quad
            }

            indices[k] = (i*LongLines) + LongLines;
            indices[k + 1] = (i*LongLines) + 1;
            indices[k + 2] = ((i + 1)*LongLines) + LongLines;

            indices[k + 3] = ((i + 1)*LongLines) + LongLines;
            indices[k + 4] = (i*LongLines) + 1;
            indices[k + 5] = ((i + 1)*LongLines) + 1;

            k += 6;
        }

        for (DWORD l = 0; l < LongLines - 1; ++l)
        {
            indices[k] = NumSphereVertices - 1;
            indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
            indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
            k += 3;
        }

        indices[k] = NumSphereVertices - 1;
        indices[k + 1] = (NumSphereVertices - 1) - LongLines;
        indices[k + 2] = NumSphereVertices - 2;

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA iinitData;

        iinitData.pSysMem = &indices[0];
        g_pDevice->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);
    }

    void Skybox::LoadShaders()
    {
        D3DCompileFromFile(L"Skybox.fx", nullptr, nullptr, "SKYMAP_VS", "vs_4_0", 0, 0, &SKYMAP_VS_Buffer, 0);
        D3DCompileFromFile(L"Skybox.fx", nullptr, nullptr, "SKYMAP_PS", "ps_4_0", 0, 0, &SKYMAP_PS_Buffer, 0);

        g_pDevice->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
        g_pDevice->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
        
        ID3D11Resource* SMTexture;
        CreateDDSTextureFromFile(g_pDevice, L"Textures/skymap.dds", &SMTexture, &smrv, 0, nullptr);

        //D3D11_TEXTURE2D_DESC SMTextureDesc;
        D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
        SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SMViewDesc.TextureCube.MostDetailedMip = 0;

        g_pDevice->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
        

        /*

        D3D11_DEPTH_STENCIL_DESC dssDesc;
        ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
        dssDesc.DepthEnable = true;
        dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        g_pDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);*/
    }

    void Skybox::Render()
    {
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        g_pDeviceContext->PSSetShaderResources(0, 1, &smrv);
        //g_pDeviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);

        // Set index buffer to this one
        g_pDeviceContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        g_pDeviceContext->VSSetShader(SKYMAP_VS, 0, 0);
        g_pDeviceContext->PSSetShader(SKYMAP_PS, 0, 0);

        // Set vertex buffer to this one
        g_pDeviceContext->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

        g_pDeviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0);
    }
}