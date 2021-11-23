#pragma once

#include <windows.h>
#include <d3d11.h>

#include "Camera.h"

class Camera;

class GfxDevice {
public:
    GfxDevice();
    ~GfxDevice();

    ID3D11Device* GetDevice() { return device; }
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }

    void Present(Camera *pCameras, unsigned int numCameras);

private:
    HWND m_Window;

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;

    ID3D11Texture2D* backBuffer;
    ID3D11RenderTargetView* backBufferView;

    ID3D11Texture2D* depthBuffer;
    ID3D11DepthStencilView* depthBufferView;

    ID3D11DepthStencilState* depthStencilState;
    ID3D11BlendState* blendState;

    ID3D11SamplerState* samplerState;
    ID3D11Buffer* quadBuffer;
};
