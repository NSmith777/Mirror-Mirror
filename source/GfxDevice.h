#pragma once

#include <windows.h>
#include <d3d11.h>

class GfxDevice {
public:
    GfxDevice(HWND window);
    ~GfxDevice();

    ID3D11Device* GetDevice() { return device; }
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }

    ID3D11RenderTargetView* GetRenderTarget() { return backBufferView; }
    ID3D11DepthStencilView* GetDepthStencil() { return depthBufferView; }

    ID3D11RasterizerState* GetRasterizerState() { return rasterizerState; }
    ID3D11SamplerState* GetSamplerState() { return samplerState; }
    ID3D11DepthStencilState* GetDepthStencilState() { return depthStencilState; }

    void InitD3DDevice();
    void InitFrameBuffers();
    void InitViewport();
    void InitRenderStates();

    void Present();

private:
    HWND m_Window;

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;

    ID3D11Texture2D* backBuffer;
    ID3D11RenderTargetView* backBufferView;

    ID3D11Texture2D* depthBuffer;
    ID3D11DepthStencilView* depthBufferView;

    ID3D11RasterizerState* rasterizerState;
    ID3D11SamplerState* samplerState;
    ID3D11DepthStencilState* depthStencilState;
};
