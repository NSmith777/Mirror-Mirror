#include "GfxDevice.h"
#include "Model.h"

#define TITLE "Mirror, Mirror"

GfxDevice::GfxDevice() {
    ////////// Init Window //////////

    WNDCLASSEXA wndClassEx = { sizeof(wndClassEx) };
    wndClassEx.lpfnWndProc = DefWindowProcA;
    wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
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
        D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &deviceContext);

    ////////// Init Framebuffers //////////

    backBufferView = NULL;
    depthBufferView = NULL;

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

    device->CreateRenderTargetView(backBuffer, nullptr, &backBufferView);

    D3D11_TEXTURE2D_DESC depthBufferDesc;

    backBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);

    device->CreateDepthStencilView(depthBuffer, nullptr, &depthBufferView);

    // The following states are for drawing the camera render targets to the screen

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

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
    device->CreateBlendState(&BlendStateDesc, &blendState);

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    device->CreateSamplerState(&samplerDesc, &samplerState);

    ////////// Init Viewport //////////

    D3D11_VIEWPORT viewport = { 0, 0, 1280, 720, 0, 1 };

    deviceContext->RSSetViewports(1, &viewport);

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

    device->CreateBuffer(&vertexBufferDesc, &vertexData, &quadBuffer);
}

void GfxDevice::Present(Camera* pCameras, unsigned int numCameras) {
    FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    deviceContext->ClearRenderTargetView(backBufferView, clearColor);
    deviceContext->ClearDepthStencilView(depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    deviceContext->OMSetRenderTargets(1, &backBufferView, depthBufferView);
    deviceContext->OMSetDepthStencilState(depthStencilState, 0);
    deviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);

    for (unsigned int i = 0; i < numCameras; i++) {
        D3D11_VIEWPORT camViewport = pCameras[i].GetViewport();
        
        Transform scrTransform;
        scrTransform.Translate(camViewport.TopLeftX - 1.0f, camViewport.TopLeftY - 1.0f, 0.0f);
        scrTransform.SetScale(camViewport.Width * 2.0f, camViewport.Height * 2.0f, 1.0f);

        Constants scrConstants;
        scrConstants.MVP = scrTransform.GetModelMatrix();

        Shader* camShader = pCameras[i].GetShader();

        camShader->SetConstants(&scrConstants);
        camShader->Use();

        ID3D11ShaderResourceView* renderTargetTex = pCameras[i].GetRenderTargetTex();

        deviceContext->PSSetShaderResources(0, 1, &renderTargetTex);
        deviceContext->PSSetSamplers(0, 1, &samplerState);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;

        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        deviceContext->IASetVertexBuffers(0, 1, &quadBuffer, &stride, &offset);

        deviceContext->Draw(4, 0);
    }

    swapChain->Present(1, 0);
}

GfxDevice::~GfxDevice() {

}
