#pragma once

#include <windows.h>
#include <d3d11.h>

#include "Camera.h"

/*
We need to use Camera in our GfxDevice class, but Camera itself
relies on GfxDevice! Therefore, we also declare Camera here, so
it becomes visible to the compiler.
*/
class Camera;

/*
Provides abstraction for the graphics backend and window instance,
and is also responsible for drawing the Camera framebuffers to the screen.

You only need to instantiate ONE GfxDevice object at the start of the program,
as it will be passed by pointer to the rest of the framework components for them to use.
In the case of Windows, GfxDevice creates and exposes a D3D device + device context.

To update the window with new frames, set up a scene with a Camera(s), draw GameObjects
to that Camera(s), then call GfxDevice's Present() function to draw the Camera's textures
to the screen.

GfxDevice draws these textures onto a 1x1 quad, however a screen shader provided to the camera
can be used to manipulate both the quad and the texture.
Refer to Camera.h for more information on the Camera class.
*/
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
