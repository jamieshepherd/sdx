#include "Graphics.h"

struct VERTEX
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
};

struct ConstantBuffer
{
    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;
};

namespace SDX
{
    //--------------------------------------------------------------------------------------
    // Graphics()
    // Graphics constructor, take in some values from Engine as pointers
    //--------------------------------------------------------------------------------------
    Graphics::Graphics()
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

    //--------------------------------------------------------------------------------------
    // void InitDirectX()
    // Initialise the DirectX device, swap chain, and buffers
    //--------------------------------------------------------------------------------------
    void Graphics::InitDirectX(UINT* m_pScreenWidth, UINT* m_pScreenHeight, HWND* m_pWindowHandle, WNDCLASSEX* m_pWindow)
    {   
        m_ScreenWidth = m_pScreenWidth;
        m_ScreenHeight = m_pScreenHeight;
        m_WindowHandle = m_pWindowHandle;
        m_Window = m_pWindow;

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

        // Create the Direct3D device
        ThrowIfFailed(D3D11CreateDevice(NULL, m_DriverType, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &direct3DDevice, &m_FeatureLevel, &direct3DDeviceContext), L"Could not create D3D device");
        

        // Swap over to ID3D11Device1
        ThrowIfFailed(direct3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pDirect3DDevice)), L"Could not swap to ID3D11Device1");

        // Swap over to ID3D11DeviceContext1
        ThrowIfFailed(direct3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pDirect3DDeviceContext)), L"Could not swap to ID3D11DeviceContext1");

        ReleaseObject(direct3DDevice);
        ReleaseObject(direct3DDeviceContext);

        // MULTISAMPLING
        UINT multiSamplingCount = 4;
        UINT multiSamplingQualityLevels;
        ThrowIfFailed(m_pDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multiSamplingCount, &multiSamplingQualityLevels), L"Could not check multisample quality levels");
        
        if (multiSamplingQualityLevels == 0) {
            //throw GameException("Unsupported multi-sampling quality");
        }

        // Swap chain descriptor
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.Width = *m_ScreenWidth;
        swapChainDesc.Height = *m_ScreenHeight;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        // Swap chain descriptor for fullscreen
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
        ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
        fullScreenDesc.RefreshRate.Numerator = 60;
        fullScreenDesc.RefreshRate.Denominator = 1;
        fullScreenDesc.Windowed = true;

        // Swap to IDXGI Device
        IDXGIDevice* dxgiDevice = nullptr;
        ThrowIfFailed(m_pDirect3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)), L"Could not swap to IDXGI device");

        // ERROR CHECK
        IDXGIAdapter *dxgiAdapter = nullptr;
        ThrowIfFailed(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter)), L"Could not get parent DXGI Device");
        
        // IF FAILED ReleaseObject(dxgiDevice)

        // Swap to IDXGIFactory 2
        IDXGIFactory2* dxgiFactory = nullptr;
        ThrowIfFailed(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory)), L"Could not swap to IDXGIFactory2");
        // IF FAILED ReleaseObject(dxgiDevice);
        // IF FAILED ReleaseObject(dxgiAdapter);

        // Create the swap chain
        ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, *m_WindowHandle, &swapChainDesc, &fullScreenDesc, nullptr, &m_pSwapChain), L"Could not create swap chain for hwnd");
        // IF FAILED ReleaseObject(dxgiDevice);
        // IF FAILED ReleaseObject(dxgiAdapter);
        // IF FAILED ReleaseObject(dxgiFactory);

        ReleaseObject(dxgiDevice);
        ReleaseObject(dxgiAdapter);
        ReleaseObject(dxgiFactory);

        // Get buffer
        ID3D11Texture2D* pBackBuffer;
        ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)), L"Could not get buffer");

        // Create render target view
        ThrowIfFailed(m_pDirect3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView), L"Could not create render target view");
        // IF FAILED ReleaseObject(backBuffer);

        ReleaseObject(pBackBuffer);

        // Depth stencil texture
        D3D11_TEXTURE2D_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.Width = *m_ScreenWidth;
        depthStencilDesc.Height = *m_ScreenHeight;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags = 0;
        //depthStencilDesc.SampleDesc.Count = multiSamplingCount;
        //depthStencilDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;

        // 2D texture which acts as our depth buffer
        ThrowIfFailed(m_pDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer), L"Could not create depth stencil texture");

        // ERROR CHECK
        // Create the depth stencil view, take in the stencil descriptor
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc.Format = depthStencilDesc.Format;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;
        ThrowIfFailed(m_pDirect3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView), L"Could not create depth stencil view");

        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(*m_ScreenWidth);
        viewport.Height = static_cast<float>(*m_ScreenHeight);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_pDirect3DDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
        m_pDirect3DDeviceContext->RSSetViewports(1, &viewport);

        LoadShaders();
        LoadMesh();
    }

    //--------------------------------------------------------------------------------------
    // void CreateShaders()
    // Compile and create shaders
    //--------------------------------------------------------------------------------------
    void Graphics::LoadShaders()
    {
        // Compile our shaders to a blob type
        ID3DBlob* pVSBlob = nullptr;
        ID3DBlob* pPSBlob = nullptr;
        D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_4_0", 0, 0, &pVSBlob, 0);
        D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_4_0", 0, 0, &pPSBlob, 0);

        // Create our shaders
        m_pDirect3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
        m_pDirect3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);

        // Describe how the input will be laid out to the buffer
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        
        UINT totalLayoutElements = ARRAYSIZE(inputElementDesc);

        m_pDirect3DDevice->CreateInputLayout(inputElementDesc, totalLayoutElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout);
        m_pDirect3DDeviceContext->IASetInputLayout(m_pInputLayout);

        ReleaseObject(pVSBlob);
        ReleaseObject(pPSBlob);
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
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        };

        // Buffer description
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;             // Write access for CPU and GPU
        bufferDesc.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(vertices);          // Size is the vertex struct times the amount of vertices
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // Use as a vertex buffer

        // Subresource data
        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = vertices;

        // Actually create the buffer
        m_pDirect3DDevice->CreateBuffer(&bufferDesc, &resourceData, &g_pVertexBuffer);

        // Set vertex buffer
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        m_pDirect3DDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

        // Create an index buffer
        WORD indices[] =
        {
            3, 1, 0,
            2, 1, 3,

            0, 5, 4,
            1, 5, 0,

            3, 4, 7,
            0, 4, 3,

            1, 6, 5,
            2, 6, 1,

            2, 7, 6,
            3, 7, 2,

            6, 4, 5,
            7, 4, 6,
        };

        // Index buffer
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        resourceData.pSysMem = indices;
        m_pDirect3DDevice->CreateBuffer(&bufferDesc, &resourceData, &g_pIndexBuffer);

        // Set index buffer
        m_pDirect3DDeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // Set primitive topology
        //m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        //m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

        // Describe constant buffers
        // TODO What does constant buffer actually do?
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(ConstantBuffer);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        // Create constant buffer
        m_pDirect3DDevice->CreateBuffer(&bufferDesc, nullptr, &g_pConstantBuffer);

        XMMATRIX WorldMatrix;
        // World matrix 1 (CUBE 1)        
        WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
        WorldMatrix = XMMatrixIdentity();
        XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);

        // View matrix
        XMMATRIX ViewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
        XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
        XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        ViewMatrix = XMMatrixLookAtLH(Eye,At,Up);
        XMStoreFloat4x4(&m_ViewMatrix, ViewMatrix);

        XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&m_ProjectionMatrix);
        ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)*m_ScreenWidth / (FLOAT)*m_ScreenHeight, 0.01f, 100.0f);
        XMStoreFloat4x4(&m_ProjectionMatrix, ProjectionMatrix);
    }

    //--------------------------------------------------------------------------------------
    // void Render()
    // Make any updates
    //--------------------------------------------------------------------------------------
    void Graphics::Update()
    {
        
    }

    void Graphics::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
    {
        g_Topology = topology;
        //m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pDirect3DDeviceContext->IASetPrimitiveTopology(topology);
        //m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    }

    //--------------------------------------------------------------------------------------
    // void Render()
    // Render anything available to the screen
    //--------------------------------------------------------------------------------------
    void Graphics::Render()
    {
        Update();

        // Update our time
        static float t = 0.0f;
        if (m_DriverType == D3D_DRIVER_TYPE_REFERENCE)
        {
            t += (float)XM_PI * 0.0125f;
        }
        else
        {
            static ULONGLONG timeStart = 0;
            ULONGLONG timeCur = GetTickCount64();
            if (timeStart == 0)
                timeStart = timeCur;
            t = (timeCur - timeStart) / 1000.0f;
        }

        XMMATRIX WorldMatrix;
        // CUBE 1 - Rotate around the origin
        WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
        WorldMatrix = XMMatrixRotationY(t);
        XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);

        // Clear the screen and stencil view
        m_pDirect3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::Black);
        m_pDirect3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        // Update variables for FIRST CUBE
        ConstantBuffer constantBuffer1;
        constantBuffer1.World = XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix));
        constantBuffer1.View = XMMatrixTranspose(XMLoadFloat4x4(&m_ViewMatrix));
        constantBuffer1.Projection = XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix));
        m_pDirect3DDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &constantBuffer1, 0, 0);

        // Set shaders to be the active shaders
        m_pDirect3DDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        m_pDirect3DDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
        m_pDirect3DDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
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