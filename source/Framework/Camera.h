//==============================================================================
// File: Camera.h
// 
// Description: Declares the Camera game object.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Shader.h"
#include "Transform.h"

using namespace DirectX;

class GfxDevice;
class Shader;

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Camera {
public:
    Camera(GfxDevice* gfxDevice, Shader* screenShader);
    ~Camera();

    void Use();

    XMMATRIX GetViewMatrix();

    XMMATRIX GetOrthoMatrix();
    XMMATRIX GetProjMatrix();

    Shader* GetShader() { return m_ScreenShader; }
    Transform* GetTransform() { return &m_Transform; }

    D3D11_VIEWPORT GetViewport() { return m_Viewport; }
    ID3D11ShaderResourceView* GetRenderTargetTex() { return m_D3DBackBufferTexView; }

    int GetPixelWidth() { return m_Width; }
    int GetPixelHeight() { return m_Height; }

    void SetClearColor(float r, float g, float b);

    XMFLOAT3 ScreenToWorldPoint(XMINT2 screenPos);

private:
    GfxDevice* m_GfxDevice;
    Shader* m_ScreenShader;

    Transform m_Transform;

    // Projection properties
    float m_ClearColor[4];
    float m_FieldOfView;
    float m_ZNear;
    float m_ZFar;
    D3D11_VIEWPORT m_Viewport;

    // Camera framebuffer
    unsigned int m_Width, m_Height;

    ID3D11Texture2D* m_D3DBackBuffer;
    ID3D11RenderTargetView* m_D3DBackBufferView;
    ID3D11ShaderResourceView* m_D3DBackBufferTexView;

    ID3D11Texture2D* m_D3DDepthBuffer;
    ID3D11DepthStencilView* m_D3DDepthBufferView;
};
