#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Shader.h"
#include "Transform.h"

using namespace DirectX;

/*
We need to declare these here and make them visible to the
compiler due to GfxDevice making use of the Camera class.
*/
class GfxDevice;
class Shader;

/*
Acts as a moveable viewpoint/"eye" in the world.
The Camera is responsible for drawing the entire scene to a texture. Afterwards,
GfxDevice is responsible for presenting the Camera's texture to the screen.

To set up a Camera, pass the program's shared GfxDevice object,
and also a (preferably screen-space) shader object.
(GfxDevice draws the texture to a 1x1 quad!)

To use the Camera in a scene, call its Use() function before drawing any of the
GameObjects you wish to display on it.
For every GameObject you draw, pass the pointer to the Camera to GameObject::Render().
*/
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

    // Framebuffer width and height, in pixels
    unsigned int m_Width, m_Height;

    ID3D11Texture2D* m_D3DBackBuffer;                   // This camera's colour buffer data
    ID3D11RenderTargetView* m_D3DBackBufferView;        // Allows us to bind buffer as a render target
    ID3D11ShaderResourceView* m_D3DBackBufferTexView;   // Allows us to bind buffer as a texture

    ID3D11Texture2D* m_D3DDepthBuffer;
    ID3D11DepthStencilView* m_D3DDepthBufferView;
};
