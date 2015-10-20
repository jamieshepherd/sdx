#include "Graphics.h"

namespace SDX
{
    //--------------------------------------------------------------------------------------
    // Graphics()
    // Graphics constructor, take in some values from Engine as pointers
    //--------------------------------------------------------------------------------------
    Graphics::Graphics(UINT* m_pScreenWidth, UINT* m_pScreenHeight, HWND* m_pWindowHandle, WNDCLASSEX* m_pWindow) :
        m_ScreenWidth(m_pScreenWidth),
        m_ScreenHeight(m_pScreenHeight),
        m_WindowHandle(m_pWindowHandle),
        m_Window(m_pWindow)
    {
        m_DriverType = D3D_DRIVER_TYPE_HARDWARE;
    }

    //--------------------------------------------------------------------------------------
    // ~Graphics()
    // Graphics destructor
    //--------------------------------------------------------------------------------------
    Graphics::~Graphics()
    {

    }

    struct VERTEX
    {
        DirectX::XMFLOAT4 pos;
        DirectX::XMFLOAT4 color;
    };

    //--------------------------------------------------------------------------------------
    // void InitDirectX()
    // Initialise the DirectX device, swap chain, and buffers
    //--------------------------------------------------------------------------------------
    void Graphics::InitDirectX()
    {            
        UINT createDeviceFlags = 0;

        #if defined(DEBUG) || defined(_DEBUG)  
            createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        ID3D11Device* direct3DDevice = nullptr;
        ID3D11DeviceContext* direct3DDeviceContext = nullptr;

        // ERROR CHECK
        D3D11CreateDevice(NULL, m_DriverType, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &direct3DDevice, &m_FeatureLevel, &direct3DDeviceContext);

        // ERROR CHECK
        direct3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pDirect3DDevice));

        // ERROR CHECK
        direct3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pDirect3DDeviceContext));

        ReleaseObject(direct3DDevice);
        ReleaseObject(direct3DDeviceContext);

        UINT multiSamplingCount = 4;
        UINT multiSamplingQualityLevels;
        m_pDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multiSamplingCount, &multiSamplingQualityLevels);
        if (multiSamplingQualityLevels == 0) {
            //throw GameException("Unsupported multi-sampling quality");
        }

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.Width = *m_ScreenWidth;
        swapChainDesc.Height = *m_ScreenHeight;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = multiSamplingCount;
        swapChainDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        IDXGIDevice* dxgiDevice = nullptr;

        // ERROR CHECK
        m_pDirect3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));

        IDXGIAdapter *dxgiAdapter = nullptr;
        HRESULT hr;

        // ERROR CHECK
        hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
        // IF FAILED ReleaseObject(dxgiDevice)

        IDXGIFactory2* dxgiFactory = nullptr;

        // ERROR CHECK
        hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory));
        // IF FAILED ReleaseObject(dxgiDevice);
        // IF FAILED ReleaseObject(dxgiAdapter);

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
        ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
        fullScreenDesc.RefreshRate.Numerator = 60;
        fullScreenDesc.RefreshRate.Denominator = 1;
        fullScreenDesc.Windowed = true;

        // ERROR CHECK
        hr = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, *m_WindowHandle, &swapChainDesc, &fullScreenDesc, nullptr, &m_pSwapChain);
        // IF FAILED ReleaseObject(dxgiDevice);
        // IF FAILED ReleaseObject(dxgiAdapter);
        // IF FAILED ReleaseObject(dxgiFactory);

        ReleaseObject(dxgiDevice);
        ReleaseObject(dxgiAdapter);
        ReleaseObject(dxgiFactory);

        ID3D11Texture2D* backBuffer;

        // ERROR CHECK
        m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

        // ERROR CHECK
        hr = m_pDirect3DDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView);
        // IF FAILED ReleaseObject(backBuffer);
        ReleaseObject(backBuffer);

        D3D11_TEXTURE2D_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.Width = *m_ScreenWidth;
        depthStencilDesc.Height = *m_ScreenHeight;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.SampleDesc.Count = multiSamplingCount;
        depthStencilDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;

        // ERROR CHECK
        // 2D texture which acts as our depth buffer
        m_pDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);

        // ERROR CHECK
        // Create the depth stencil view, take in the stencil descriptor
        m_pDirect3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);

        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(*m_ScreenWidth);
        viewport.Height = static_cast<float>(*m_ScreenHeight);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_pDirect3DDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
        m_pDirect3DDeviceContext->RSSetViewports(1, &viewport);

        CreateShaders();
        LoadMesh();
    }

    //--------------------------------------------------------------------------------------
    // void CreateShaders()
    // Compile and create shaders
    //--------------------------------------------------------------------------------------
    void Graphics::CreateShaders()
    {
        // Compile our shaders to a blob type
        ID3D10Blob *VS, *PS;
        D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_4_0", 0, 0, &VS, 0);
        D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_4_0", 0, 0, &PS, 0);

        // Create our shaders
        m_pDirect3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &m_pVertexShader);
        m_pDirect3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &m_pPixelShader);

        // Describe how the input will be laid out to the buffer
        
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT totalLayoutElements = ARRAYSIZE(inputElementDesc);

        m_pDirect3DDevice->CreateInputLayout(inputElementDesc, totalLayoutElements, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
    }

    //--------------------------------------------------------------------------------------
    // void LoadMesh()
    // Load a mesh, soon to take OBJ files
    //--------------------------------------------------------------------------------------
    void Graphics::LoadMesh()
    {
        // Define some vertices
        VERTEX vertices[] =
        {
            { DirectX::XMFLOAT4(-1.0f, +1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, +1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, +1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(-1.0f, +1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

            { DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, -1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(-1.0f, -1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },

            { DirectX::XMFLOAT4(-1.0f, -1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { DirectX::XMFLOAT4(-1.0f, +1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(-1.0f, +1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

            { DirectX::XMFLOAT4(+1.0f, -1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, +1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, +1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

            { DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, +1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(-1.0f, +1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

            { DirectX::XMFLOAT4(-1.0f, -1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, -1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { DirectX::XMFLOAT4(+1.0f, +1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { DirectX::XMFLOAT4(-1.0f, +1.0f, +1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
        };

        // Buffer description
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(vertices);          // Size is the vertex struct times the amount of vertices
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;             // Write access for CPU and GPU
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // Use as a vertex buffer

        // Subresource data
        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = vertices;

        // Actually create the buffer
        m_pDirect3DDevice->CreateBuffer(&bufferDesc, &resourceData, &g_pVertexBuffer);

        // Create an index buffer
        WORD indices[] =
        {
            3, 1, 0, 2, 1, 3,
            6, 4, 5, 7, 4, 6,
            11, 9, 8, 10, 9, 11,
            14, 12, 13, 15, 12, 14,
            19, 17, 16, 18, 17, 19,
            22, 20, 21, 23, 20, 22
        };

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);        // 36 vertices needed for 12 triangles in a triangle list
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        D3D11_SUBRESOURCE_DATA indexResourceData;
        ZeroMemory(&indexResourceData, sizeof(indexResourceData));
        indexResourceData.pSysMem = indices;

        m_pDirect3DDevice->CreateBuffer(&indexBufferDesc, &indexResourceData, &g_pIndexBuffer);

        // Texture sampler state here

        // Describe constant buffers
        // TODO What does constant buffer actually do
        D3D11_BUFFER_DESC constantBufferDesc;
        ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
        constantBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        // Create constant buffer for VIEW
        m_pDirect3DDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_pViewConstantBuffer);

        // Create constant buffer for PROJECTION
        m_pDirect3DDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_pProjectionConstantBuffer);

        // Create constant buffer for WORLD
        m_pDirect3DDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_pWorldConstantBuffer);

        m_ViewMatrix = DirectX::XMMatrixIdentity();
        m_ProjectionMatrix = DirectX::XMMatrixPerspectiveLH(DirectX::XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);

        m_ViewMatrix = DirectX::XMMatrixTranspose(m_ViewMatrix);
        m_ProjectionMatrix = DirectX::XMMatrixTranspose(m_ProjectionMatrix);
    }

    //--------------------------------------------------------------------------------------
    // void Render()
    // Make any updates
    //--------------------------------------------------------------------------------------
    void Graphics::Update()
    {
        
    }

    //--------------------------------------------------------------------------------------
    // void Render()
    // Render anything available to the screen
    //--------------------------------------------------------------------------------------
    void Graphics::Render()
    {
        Update();

        // Clear the screen and stencil view
        DirectX::XMVECTORF32 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        m_pDirect3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&ClearColor));
        m_pDirect3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        // Select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        
        // Set input layout and buffers
        m_pDirect3DDeviceContext->IASetInputLayout(m_pInputLayout);
        m_pDirect3DDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
        m_pDirect3DDeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Set shaders to be the active shaders
        m_pDirect3DDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        m_pDirect3DDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

        // Matrix shit
        /*DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&m_WorldMatrix);
        DirectX::XMMATRIX wvp = worldMatrix;
        wvp = DirectX::XMMatrixTranspose(wvp);
        DirectX::XMStoreFloat4x4(&m_CBufferPerObject.WorldViewProjection, wvp);*/

        DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.7f, 0.7f);
        DirectX::XMMATRIX translationMat = DirectX::XMMatrixTranslation(0.0f, 0.0f, 6.0f);

        m_WorldMatrix = rotationMat * translationMat;
        m_WorldMatrix = DirectX::XMMatrixTranspose(m_WorldMatrix);

        // What does this do?
        m_pDirect3DDeviceContext->UpdateSubresource(g_pWorldConstantBuffer, 0, nullptr, &m_WorldMatrix, 0, 0);
        m_pDirect3DDeviceContext->UpdateSubresource(g_pViewConstantBuffer, 0, nullptr, &m_ViewMatrix, 0, 0);
        m_pDirect3DDeviceContext->UpdateSubresource(g_pProjectionConstantBuffer, 0, nullptr, &m_ProjectionMatrix, 0, 0);

        // Set constant buffers
        m_pDirect3DDeviceContext->VSSetConstantBuffers(0, 1, &g_pWorldConstantBuffer);
        m_pDirect3DDeviceContext->VSSetConstantBuffers(1, 1, &g_pViewConstantBuffer);
        m_pDirect3DDeviceContext->VSSetConstantBuffers(2, 1, &g_pProjectionConstantBuffer);

        // Draw vertex buffer to the back buffer
        m_pDirect3DDeviceContext->DrawIndexed(36, 0, 0);

        // Present
        m_pSwapChain->Present(0, 0);
    }

    //--------------------------------------------------------------------------------------
    // void Shutdown()
    // Shutdown everything from the Graphics class
    //--------------------------------------------------------------------------------------
    void Graphics::Shutdown()
    {
        ReleaseObject(g_pVertexBuffer);
        ReleaseObject(g_pIndexBuffer);
        ReleaseObject(m_pInputLayout);
        ReleaseObject(m_pVertexShader);
        ReleaseObject(m_pPixelShader);
        ReleaseObject(m_pRenderTargetView);
        ReleaseObject(m_pDepthStencilView);
        ReleaseObject(m_pSwapChain);
        ReleaseObject(m_pDepthStencilBuffer);
        ReleaseObject(m_pDirect3DDeviceContext);
        ReleaseObject(m_pDirect3DDevice);
        ReleaseObject(g_pWorldConstantBuffer);
        ReleaseObject(g_pViewConstantBuffer);
        ReleaseObject(g_pProjectionConstantBuffer);
    }
}