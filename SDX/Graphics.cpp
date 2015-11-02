#include "Graphics.h"

//struct ConstantBuffer
//{
//    XMMATRIX World;
//    XMMATRIX View;
//    XMMATRIX Projection;
//};

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
        //UINT multiSamplingCount;
        //UINT multiSamplingQualityLevels;
        //ThrowIfFailed(m_pDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multiSamplingCount, &multiSamplingQualityLevels), L"Could not check multisample quality levels");
        
        //if (multiSamplingQualityLevels == 0) {
            //throw GameException("Unsupported multi-sampling quality");
        //}

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
        LoadRasterizers();
        LoadMesh();

        LoadFont();

        m_pDirect3DDeviceContext->RSSetState(rs_Solid);
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

        // Set active shaders
        m_pDirect3DDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        m_pDirect3DDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

        // Describe how the input will be laid out to the buffer
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            //{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        
        UINT totalLayoutElements = ARRAYSIZE(inputElementDesc);

        m_pDirect3DDevice->CreateInputLayout(inputElementDesc, totalLayoutElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout);
        m_pDirect3DDeviceContext->IASetInputLayout(m_pInputLayout);

        ReleaseObject(pVSBlob);
        ReleaseObject(pPSBlob);
    }

    void Graphics::LoadRasterizers()
    {
        D3D11_RASTERIZER_DESC rsDesc;
        ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
        rsDesc.FillMode = D3D11_FILL_WIREFRAME;
        rsDesc.CullMode = D3D11_CULL_NONE;
        ThrowIfFailed(m_pDirect3DDevice->CreateRasterizerState(&rsDesc, &rs_WireFrame), L"Couldn't create rasterizer state Wireframe");
        
        ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
        rsDesc.FillMode = D3D11_FILL_SOLID;
        rsDesc.CullMode = D3D11_CULL_NONE;
        ThrowIfFailed(m_pDirect3DDevice->CreateRasterizerState(&rsDesc, &rs_Solid), L"Couldn't create rasterizer state Solid");
    }

    void Graphics::SetRasterizer(int state)
    {
        switch (state) {
            case 0:
                rs_State = 0;
                m_pDirect3DDeviceContext->RSSetState(rs_WireFrame);
                break;
            case 1:
                rs_State = 1;
                m_pDirect3DDeviceContext->RSSetState(rs_Solid);
                break;
            default:
                rs_State = 1;
                m_pDirect3DDeviceContext->RSSetState(rs_Solid);
        }
    }

    //--------------------------------------------------------------------------------------
    // void LoadMesh()
    // Load a mesh, soon to take OBJ files
    //--------------------------------------------------------------------------------------
    void Graphics::LoadMesh()
    {
        g_Model1 = new Model(m_pDirect3DDevice, m_pDirect3DDeviceContext);
        g_Model1->LoadModel("Models/Cube.obj", true, false);

        //objModel.LoadModel("Models/Cube.obj", true, false);
        //objModel.LoadModel("Models/Teapot.obj", true, false);
        //objModel.LoadModel("Models/Tiger.obj", true, true);

        // Load texture file
        //std::wstring textureName = L"Models/Tiger.jpg";
        //ThrowIfFailed(CreateWICTextureFromFile(m_pDirect3DDevice, m_pDirect3DDeviceContext, textureName.c_str(), nullptr, &m_MeshTexture), L"Couldn't load texture from file");

        // Create a texture sampler
        /*D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        ThrowIfFailed(m_pDirect3DDevice->CreateSamplerState(&samplerDesc, &m_MeshTextureSamplerState), L"Couldn't create sampler state");*/

        // Describe constant buffers
        // TODO What does constant buffer actually do?
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(cbPerObject);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        // Create constant buffer
        m_pDirect3DDevice->CreateBuffer(&bufferDesc, nullptr, &cbPerObjectBuffer);

        // We get cam view from g_Camera in Update()

        // Set primitive topology
        m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Set the Projection matrix
        XMMATRIX t_CamProjection;
        t_CamProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)*m_ScreenWidth / (FLOAT)*m_ScreenHeight, 1.0f, 1000.0f);
        XMStoreFloat4x4(&m_CamProjection, t_CamProjection);
    }

    //--------------------------------------------------------------------------------------
    // void Render()
    // Make any updates
    //--------------------------------------------------------------------------------------
    void Graphics::Update(SDX::Camera* g_Camera)
    {
        m_CamView = g_Camera->GetViewMatrix();
    }

    void Graphics::LoadFont()
    {
        
    }

    void Graphics::DrawString(char* text, float positionX, float positionY)
    {
        
    }

    XMMATRIX Graphics::GetWVP()
    {
        XMMATRIX t_WVP, t_World, t_View, t_Projection;
        t_WVP = XMLoadFloat4x4(&m_WVP);
        t_World = XMLoadFloat4x4(&m_World);
        t_View = XMLoadFloat4x4(&m_CamView);
        t_Projection = XMLoadFloat4x4(&m_CamProjection);
        t_WVP = t_World * t_View * t_Projection;
        XMStoreFloat4x4(&m_WVP, t_WVP);

        return XMMatrixTranspose(t_WVP);
    }

    //--------------------------------------------------------------------------------------
    // void Render()
    // Render anything available to the screen
    //--------------------------------------------------------------------------------------
    void Graphics::Render()
    {
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

        // CUBE - Rotate around the origin
        XMMATRIX t_World;
        t_World = XMLoadFloat4x4(&m_World);
        t_World = XMMatrixRotationY(t);
        XMStoreFloat4x4(&m_World, t_World);

        cbPerObject cbPerObj;
        cbPerObj.WVP = GetWVP();

        // Clear the screen and stencil view
        m_pDirect3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::Black);
        m_pDirect3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        // Update variables for FIRST CUBE
        m_pDirect3DDeviceContext->UpdateSubresource(cbPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);

        // Set shaders to be the active shaders
        m_pDirect3DDeviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
        //m_pDirect3DDeviceContext->PSSetShaderResources(0, 1, &m_MeshTexture);
        //m_pDirect3DDeviceContext->PSSetSamplers(0, 1, &m_MeshTextureSamplerState);

        g_Model1->DrawIndexed();

        // Present
        m_pSwapChain->Present(0, 0);
    }

    //--------------------------------------------------------------------------------------
    // void Shutdown()
    // Shutdown everything from the Graphics class
    //--------------------------------------------------------------------------------------
    void Graphics::Shutdown()
    {
        ReleaseObject(rs_WireFrame);
        ReleaseObject(rs_Solid);
        ReleaseObject(m_pInputLayout);
        ReleaseObject(m_pVertexShader);
        ReleaseObject(m_pPixelShader);
        ReleaseObject(m_pRenderTargetView);
        ReleaseObject(m_pDepthStencilView);
        ReleaseObject(m_pSwapChain);
        ReleaseObject(m_pDepthStencilBuffer);
        ReleaseObject(m_pDirect3DDeviceContext);
        ReleaseObject(m_pDirect3DDevice);       
    }
}