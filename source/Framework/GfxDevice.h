//==============================================================================
// File: GfxDevice.h
// 
// Description: Declares the graphics device object.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <d3d11.h>

#include "Camera.h"

class Camera;

//=============================================================================
// 
// Synopsis:    Provides abstraction for the graphics backend
//              and window instance, and is also responsible for drawing the
//              Camera framebuffers to the screen.
// 
//=============================================================================
class GfxDevice {
public:
    GfxDevice();
    ~GfxDevice();

    ID3D11Device* GetDevice() { return m_D3DDevice; }
    ID3D11DeviceContext* GetDeviceContext() { return m_D3DDeviceContext; }

    // Retrieve elapsed system time in seconds
    float GetTime() { return timeGetTime() / 1000.0f; }

    void Present(Camera *pCameras, unsigned int numCameras);

private:
    HWND m_Window;

    ID3D11Device* m_D3DDevice;
    ID3D11DeviceContext* m_D3DDeviceContext;
    IDXGISwapChain* m_D3DSwapChain;

    ID3D11Texture2D* m_D3DBackBuffer;
    ID3D11RenderTargetView* m_D3DBackBufferView;

    ID3D11Texture2D* m_D3DDepthBuffer;
    ID3D11DepthStencilView* m_D3DDepthBufferView;

    ID3D11DepthStencilState* m_D3DDepthStencilState;
    ID3D11BlendState* m_D3DBlendState;

    ID3D11SamplerState* m_D3DSamplerState;
    ID3D11Buffer* m_D3DQuadVB;
};
