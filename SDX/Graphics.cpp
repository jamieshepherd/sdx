#include "Graphics.h"

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
        m_ScreenWidth  = m_pScreenWidth;
        m_ScreenHeight = m_pScreenHeight;
        m_WindowHandle = m_pWindowHandle;
        m_Window       = m_pWindow;

        UINT createDeviceFlags = 0;

        #if defined(DEBUG) || defined(_DEBUG)  
            createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        ID3D11Device*        direct3DDevice        = nullptr;
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
        swapChainDesc.Width              = *m_ScreenWidth;
        swapChainDesc.Height             = *m_ScreenHeight;
        swapChainDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count   = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount        = 1;
        swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;

        // Swap chain descriptor for fullscreen
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
        ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
        fullScreenDesc.RefreshRate.Numerator   = 60;
        fullScreenDesc.RefreshRate.Denominator = 1;
        fullScreenDesc.Windowed                = true;

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
        depthStencilDesc.Width              = *m_ScreenWidth;
        depthStencilDesc.Height             = *m_ScreenHeight;
        depthStencilDesc.MipLevels          = 1;
        depthStencilDesc.ArraySize          = 1;
        depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags     = 0;
        depthStencilDesc.MiscFlags          = 0;
        //depthStencilDesc.SampleDesc.Count = multiSamplingCount;
        //depthStencilDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;
        depthStencilDesc.SampleDesc.Count   = 1;
        depthStencilDesc.SampleDesc.Quality = 0;

        // 2D texture which acts as our depth buffer
        ThrowIfFailed(m_pDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer), L"Could not create depth stencil texture");

        // ERROR CHECK
        // Create the depth stencil view, take in the stencil descriptor
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc.Format             = depthStencilDesc.Format;
        depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        // Create the view
        ThrowIfFailed(m_pDirect3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView), L"Could not create depth stencil view");

        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width    = static_cast<float>(*m_ScreenWidth);
        viewport.Height   = static_cast<float>(*m_ScreenHeight);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_pDirect3DDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
        m_pDirect3DDeviceContext->RSSetViewports(1, &viewport);

        LoadShaders();
        LoadGrid();
        LoadSkybox();
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
        

        // Describe how the input will be laid out to the buffer
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        
        UINT totalLayoutElements = ARRAYSIZE(inputElementDesc);

        m_pDirect3DDevice->CreateInputLayout(inputElementDesc, totalLayoutElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout);
        m_pDirect3DDeviceContext->IASetInputLayout(m_pInputLayout);

        ReleaseObject(pVSBlob);
        ReleaseObject(pPSBlob);
    }

    void Graphics::LoadGrid()
    {
        g_Grid = new Grid(m_pDirect3DDevice, m_pDirect3DDeviceContext);
        g_Grid->Init();
    }

    void Graphics::LoadSkybox()
    {
        g_Skybox = new Skybox(m_pDirect3DDevice, m_pDirect3DDeviceContext);
        //g_Skybox->CreateSphere(10, 10);
        //g_Skybox->LoadShaders();
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
        // Example models: Cube, Tiger(Textured), Teapot, Teddy
        g_Model1 = new Model(m_pDirect3DDevice, m_pDirect3DDeviceContext);
        g_Model1->LoadModel("Models/Tiger.obj", L"Models/Tiger.jpg", false, true);

        //g_Model2 = new Model(m_pDirect3DDevice, m_pDirect3DDeviceContext);
        //g_Model2->LoadModel("Models/Tiger.obj", L"Models/Tiger.jpg", false, true);

        g_Terrain = new Terrain(m_pDirect3DDevice, m_pDirect3DDeviceContext);
        g_Terrain->Init();

        // Describe constant buffers
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(cbPerObject);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        // Create constant buffer
        m_pDirect3DDevice->CreateBuffer(&bufferDesc, nullptr, &cbPerObjectBuffer);

        // Set primitive topology
        m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Set Camera View matrix

        // Set the Camera Projection matrix
        XMMATRIX t_CamProjection;
        t_CamProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (FLOAT)*m_ScreenWidth / (FLOAT)*m_ScreenHeight, 0.1f, 10000.0f);
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

    void Graphics::LoadTerrain()
    {
        
    }

    void Graphics::LoadFont()
    {
        CreateDDSTextureFromFile(m_pDirect3DDevice, L"Textures/font.dds", nullptr, &m_FontTexture);

        D3D11_SAMPLER_DESC colorMapDesc;
        ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
        colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

        m_pDirect3DDevice->CreateSamplerState(&colorMapDesc, &m_FontTextureSamplerState);

        D3D11_BUFFER_DESC vertexDesc;
        ZeroMemory(&vertexDesc, sizeof(vertexDesc));
        vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        const int sizeOfSprite = sizeof(VERTEX) * 6;
        const int maxLetters = 24;

        vertexDesc.ByteWidth = sizeOfSprite * maxLetters;

        m_pDirect3DDevice->CreateBuffer(&vertexDesc, 0, &g_pFontVertexBuffer);
    }

    void Graphics::DrawString(char* text, float positionX, float positionY)
    {
        m_pDirect3DDeviceContext->PSSetShaderResources(0, 1, &m_FontTexture);
        m_pDirect3DDeviceContext->PSSetSamplers(0, 1, &m_FontTextureSamplerState);

        // Size in bytes for a single sprite.
        const int sizeOfSprite = sizeof(VERTEX) * 6;

        // Demo's dynamic buffer setup for max of 24 letters.
        const int maxLetters = 24;

        int length = strlen(text);

        // Clamp for strings too long.
        if (length > maxLetters)
            length = maxLetters;

        // Char's width on screen.
        float charWidth = 32.0f / 800.0f;

        // Char's height on screen.
        float charHeight = 32.0f / 640.0f;

        // Char's texel width.
        float texelWidth = 32.0f / 864.0f;

        // verts per-triangle (3) * total triangles (2) = 6.
        const int verticesPerLetter = 6;

        D3D11_MAPPED_SUBRESOURCE mapResource;
        HRESULT d3dResult = m_pDirect3DDeviceContext->Map(g_pFontVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
        
        // Point to our vertex buffer's internal data.
        VERTEX *spritePtr = (VERTEX*)mapResource.pData;

        const int indexA = static_cast<char>('A');
        const int indexZ = static_cast<char>('Z');

        for (int i = 0; i < length; ++i)
        {
            float thisStartX = positionX + (charWidth * static_cast<float>(i));
            float thisEndX = thisStartX + charWidth;
            float thisEndY = positionY + charHeight;

            spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
            spritePtr[1].pos = XMFLOAT3(thisEndX, positionY, 1.0f);
            spritePtr[2].pos = XMFLOAT3(thisStartX, positionY, 1.0f);
            spritePtr[3].pos = XMFLOAT3(thisStartX, positionY, 1.0f);
            spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
            spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

            int texLookup = 0;
            int letter = static_cast<char>(text[i]);

            if (letter < indexA || letter > indexZ)
            {
                // Grab one index past Z, which is a blank space in the texture.
                texLookup = (indexZ - indexA) + 1;
            }
            else
            {
                // A = 0, B = 1, Z = 25, etc.
                texLookup = (letter - indexA);
            }

            float tuStart = 0.0f + (texelWidth * static_cast<float>(texLookup));
            float tuEnd = tuStart + texelWidth;

            spritePtr[0].tex = XMFLOAT2(tuEnd, 0.0f);
            spritePtr[1].tex = XMFLOAT2(tuEnd, 1.0f);
            spritePtr[2].tex = XMFLOAT2(tuStart, 1.0f);
            spritePtr[3].tex = XMFLOAT2(tuStart, 1.0f);
            spritePtr[4].tex = XMFLOAT2(tuStart, 0.0f);
            spritePtr[5].tex = XMFLOAT2(tuEnd, 0.0f);

            spritePtr += 6;
        }

        m_pDirect3DDeviceContext->Unmap(g_pFontVertexBuffer, 0);
        m_pDirect3DDeviceContext->Draw(6 * length, 0);
    }

    XMMATRIX Graphics::GetWVP(XMFLOAT4X4* world)
    {
        XMMATRIX t_WVP, t_World, t_View, t_Projection;
        t_WVP = XMLoadFloat4x4(&m_WVP);
        t_World = XMLoadFloat4x4(world);
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
        // Clear the screen and stencil view
        //m_pDirect3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::DarkSeaGreen);
        m_pDirect3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::Black);
        m_pDirect3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        // Set the constant buffer
        m_pDirect3DDeviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

        // Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
        m_pDirect3DDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Set shaders for meshes
        m_pDirect3DDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        m_pDirect3DDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

        //// Update our time
        //static float t = 0.0f;
        //static ULONGLONG timeStart = 0;
        //ULONGLONG timeCur = GetTickCount64();
        //if (timeStart == 0)
        //    timeStart = timeCur;
        //t = (timeCur - timeStart) / 1000.0f;

        // ---
        // Terrain
        // ---

        XMMATRIX t_GroundWorld = XMMatrixIdentity();
        XMMATRIX t_GroundScale = XMMatrixScaling(500.0f, 10.0f, 500.0f);
        XMMATRIX t_GroundTranslation = XMMatrixTranslation(0.0f, 10.0f, 0.0f);
        t_GroundWorld = t_GroundScale * t_GroundTranslation;
        XMStoreFloat4x4(&m_GroundWorld, t_GroundWorld);

        cbPerObject ground;
        ground.WVP = GetWVP(&m_GroundWorld);

        m_pDirect3DDeviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &ground, 0, 0);
        m_pDirect3DDeviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

        g_Terrain->Render();

        // ---
        // Object #1 (Mesh)
        // ---
        XMMATRIX t_World = XMMatrixIdentity();
        XMMATRIX t_Spin = XMMatrixRotationY(-1.00f);
        XMMATRIX t_Scale = XMMatrixScaling(0.004f, 0.004f, 0.004f); // Original file is pretty large
        t_World = XMLoadFloat4x4(&m_Object1World);
        t_World = t_Scale * t_Spin;
        XMStoreFloat4x4(&m_Object1World, t_World);

        cbPerObject cb1;
        cb1.WVP = GetWVP(&m_Object1World);

        m_pDirect3DDeviceContext->UpdateSubresource(cbPerObjectBuffer, 0, nullptr, &cb1, 0, 0);

        g_Model1->DrawIndexed();

        //DrawString("HELLO WORLD", -0.2f, 0.0f);

        // Present to the screen this frame
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