#include "GfxDevice.h"

GfxDevice::GfxDevice(HWND window) {
    m_Window = window;

    backBufferView = NULL;
    depthBufferView = NULL;

    ////////// Init D3D //////////

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = 1280;                            // use window width
    scd.BufferDesc.Height = 720;                            // use window height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = m_Window;                            // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &deviceContext);

    ////////// Init Framebuffers //////////

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

    device->CreateRenderTargetView(backBuffer, nullptr, &backBufferView);

    D3D11_TEXTURE2D_DESC depthBufferDesc;

    backBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);

    device->CreateDepthStencilView(depthBuffer, nullptr, &depthBufferView);

    ////////// Init Viewport //////////

    D3D11_VIEWPORT viewport = { 0, 0, 1280, 720, 0, 1 };

    deviceContext->RSSetViewports(1, &viewport);

    ////////// Init Render States //////////

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    device->CreateSamplerState(&samplerDesc, &samplerState);

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
}

void GfxDevice::Present() {
    swapChain->Present(1, 0);
}

GfxDevice::~GfxDevice() {

}
