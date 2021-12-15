//==============================================================================
// File: GfxDevice.cpp
// 
// Description: Implements the graphics device object.
// 
//==============================================================================

#include "GfxDevice.h"
#include "Model.h"

#define TITLE "Mirror, Mirror"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
        ExitProcess(0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}

//=============================================================================
// GfxDevice::GfxDevice
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
GfxDevice::GfxDevice() {
    ////////// Init Window //////////

    WNDCLASSEXA wndClassEx = { sizeof(wndClassEx) };
    wndClassEx.lpfnWndProc = WndProc;
    wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClassEx.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), L"MAINICON", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
    wndClassEx.lpszClassName = TITLE;

    RegisterClassExA(&wndClassEx);

    m_Window = CreateWindowExA(NULL, TITLE, TITLE, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(m_Window, SW_SHOWDEFAULT);

    ////////// Init D3D //////////
    
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = 0;                               // use window width
    scd.BufferDesc.Height = 0;                              // use window height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = m_Window;                            // the window to be used
    scd.SampleDesc.Count = 1;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
        D3D11_SDK_VERSION, &scd, &m_D3DSwapChain, &m_D3DDevice, NULL, &m_D3DDeviceContext);

    ////////// Init Framebuffers //////////

    m_D3DBackBufferView = NULL;
    m_D3DDepthBufferView = NULL;

    m_D3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_D3DBackBuffer);

    m_D3DDevice->CreateRenderTargetView(m_D3DBackBuffer, nullptr, &m_D3DBackBufferView);

    D3D11_TEXTURE2D_DESC depthBufferDesc;

    m_D3DBackBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    m_D3DDevice->CreateTexture2D(&depthBufferDesc, nullptr, &m_D3DDepthBuffer);

    m_D3DDevice->CreateDepthStencilView(m_D3DDepthBuffer, nullptr, &m_D3DDepthBufferView);

    // The following states are for drawing the camera render targets to the screen

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_D3DDepthStencilState);

    D3D11_BLEND_DESC BlendStateDesc = {};
    BlendStateDesc.AlphaToCoverageEnable = FALSE;
    BlendStateDesc.IndependentBlendEnable = FALSE;
    BlendStateDesc.RenderTarget[0].BlendEnable = FALSE;
    BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_D3DDevice->CreateBlendState(&BlendStateDesc, &m_D3DBlendState);

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    m_D3DDevice->CreateSamplerState(&samplerDesc, &m_D3DSamplerState);

    ////////// Init Viewport //////////

    D3D11_VIEWPORT viewport = { 0, 0, 1280, 720, 0, 1 };

    m_D3DDeviceContext->RSSetViewports(1, &viewport);

    ////////// Init Quad Buffer //////////

    static const Vertex quadVerts[] = {
        { { 0.0f, 0.0f, 0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 0.0f, 1.0f, 0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 1.0f, 1.0f, 0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
    };

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(quadVerts);
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = { quadVerts };

    m_D3DDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_D3DQuadVB);
}

//=============================================================================
// GfxDevice::Present
//=============================================================================
// 
// Description: 
// 
// Parameters:	[Camera *]      Array of cameras to draw to the screen
//              [unsigned int]  Number of cameras in the array to draw
// 
// Return:      N/A
// 
//=============================================================================
void GfxDevice::Present(Camera* pCameras, unsigned int numCameras) {
    FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    m_D3DDeviceContext->ClearRenderTargetView(m_D3DBackBufferView, clearColor);
    m_D3DDeviceContext->ClearDepthStencilView(m_D3DDepthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    m_D3DDeviceContext->OMSetRenderTargets(1, &m_D3DBackBufferView, m_D3DDepthBufferView);
    m_D3DDeviceContext->OMSetDepthStencilState(m_D3DDepthStencilState, 0);
    m_D3DDeviceContext->OMSetBlendState(m_D3DBlendState, nullptr, 0xffffffff);

    for (unsigned int i = 0; i < numCameras; i++) {
        D3D11_VIEWPORT camViewport = pCameras[i].GetViewport();
        
        Transform scrTransform;
        scrTransform.Translate({ camViewport.TopLeftX - 1.0f, camViewport.TopLeftY - 1.0f, 0.0f });
        scrTransform.SetScale({ camViewport.Width * 2.0f, camViewport.Height * 2.0f, 1.0f });

        Constants scrConstants;
        scrConstants.MVP = scrTransform.GetModelMatrix();

        Shader* camShader = pCameras[i].GetShader();

        camShader->SetConstants(&scrConstants);
        camShader->Use();

        ID3D11ShaderResourceView* renderTargetTex = pCameras[i].GetRenderTargetTex();

        m_D3DDeviceContext->PSSetShaderResources(0, 1, &renderTargetTex);
        m_D3DDeviceContext->PSSetSamplers(0, 1, &m_D3DSamplerState);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;

        m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        m_D3DDeviceContext->IASetVertexBuffers(0, 1, &m_D3DQuadVB, &stride, &offset);

        m_D3DDeviceContext->Draw(4, 0);
    }

    m_D3DSwapChain->Present(1, 0);
}

//=============================================================================
// GfxDevice::~GfxDevice
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
GfxDevice::~GfxDevice() {
    DestroyWindow(m_Window);

    m_D3DDevice->Release();
    m_D3DDeviceContext->Release();
    m_D3DSwapChain->Release();

    m_D3DBackBuffer->Release();
    m_D3DBackBufferView->Release();

    m_D3DDepthBuffer->Release();
    m_D3DDepthBufferView->Release();

    m_D3DDepthStencilState->Release();
    m_D3DBlendState->Release();

    m_D3DSamplerState->Release();
    m_D3DQuadVB->Release();
}
