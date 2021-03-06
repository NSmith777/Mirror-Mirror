#include "Camera.h"
#include "Math.h"

using namespace Math;

//=============================================================================
// Camera::Camera
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[GfxDevice *]	Graphics device object
//				[Shader *]		Screen-space shader used to draw the game scene
//								before presenting the current frame
// 
// Return:      N/A
// 
//=============================================================================
Camera::Camera(GfxDevice* gfxDevice, Shader *screenShader) {
    m_GfxDevice = gfxDevice;
    m_ScreenShader = screenShader;
    
    m_D3DBackBufferView = NULL;
    m_D3DBackBufferTexView = NULL;
    m_D3DDepthBufferView = NULL;

    //
    // Set default camera pixel dimensions
    //
    m_Width = 1280;
    m_Height = 720;

    //
    // Set default clear colour to black
    //
    m_ClearColor[0] = 0.0f;
    m_ClearColor[1] = 0.0f;
    m_ClearColor[2] = 0.0f;
    m_ClearColor[3] = 1.0f;

    //
    // Set viewport to fit the entire screen
    //
    m_Viewport.TopLeftX = 0.0f;
    m_Viewport.TopLeftY = 0.0f;
    m_Viewport.Width = 1.0f;
    m_Viewport.Height = 1.0f;

    //
    // Finally, set default projection properties
    //
    m_FieldOfView = 60.0f;
    m_ZNear = 0.1f;
    m_ZFar = 1000.0f;

    //
    // Setup colour buffer texture
    //
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = m_Width;
    textureDesc.Height = m_Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 4;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    m_GfxDevice->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_D3DBackBuffer);

    //
    // This render target view object allows us to bind this camera's colour buffer
    // to the graphics backend later on, as an active render target to draw to.
    //
    CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2DMS, textureDesc.Format);

    m_GfxDevice->GetDevice()->CreateRenderTargetView(m_D3DBackBuffer, &renderTargetViewDesc, &m_D3DBackBufferView);

    //
    // Similarly, we can setup this shader resource view object to bind
    // this camera's colour buffer to a shader, to sample as a texture.
    //
    CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(D3D11_SRV_DIMENSION_TEXTURE2DMS, textureDesc.Format);

    m_GfxDevice->GetDevice()->CreateShaderResourceView(m_D3DBackBuffer, &shaderResourceViewDesc, &m_D3DBackBufferTexView);

    //
    // Setup depth buffer texture; we can just inherit from the colour buffer
    // properties to bypass most of the manual setup.
    //
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    m_D3DBackBuffer->GetDesc(&depthBufferDesc);

    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.SampleDesc.Count = 4;
    depthBufferDesc.SampleDesc.Quality = 0;

    m_GfxDevice->GetDevice()->CreateTexture2D(&depthBufferDesc, nullptr, &m_D3DDepthBuffer);

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS, depthBufferDesc.Format);

    m_GfxDevice->GetDevice()->CreateDepthStencilView(m_D3DDepthBuffer, &depthStencilViewDesc, &m_D3DDepthBufferView);
}

//=============================================================================
// Camera::Use
//=============================================================================
// 
// Description: Binds this camera's framebuffers to the graphics backend.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Camera::Use() {
    m_GfxDevice->GetDeviceContext()->ClearRenderTargetView(m_D3DBackBufferView, m_ClearColor);
    m_GfxDevice->GetDeviceContext()->ClearDepthStencilView(m_D3DDepthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    m_GfxDevice->GetDeviceContext()->OMSetRenderTargets(1, &m_D3DBackBufferView, m_D3DDepthBufferView);
}

//=============================================================================
// Camera::GetViewMatrix
//=============================================================================
// 
// Description: Constructs a view matrix using this camera's transformations.
// 
// Parameters:	N/A
// 
// Return:      [XMMATRIX]	This camera's view matrix
// 
//=============================================================================
XMMATRIX Camera::GetViewMatrix() {
    XMFLOAT3 position = m_Transform.GetPosition();
    XMFLOAT3 rotation = m_Transform.GetRotation();

    XMMATRIX translateMtx = XMMatrixTranslation(-position.x, -position.y, -position.z);
    XMMATRIX rotateZMtx = XMMatrixRotationNormal({ 0, 0, 1 }, -rotation.z);
    XMMATRIX rotateYMtx = XMMatrixRotationNormal({ 0, 1, 0 }, -rotation.y);
    XMMATRIX rotateXMtx = XMMatrixRotationNormal({ 1, 0, 0 }, -rotation.x);

    return translateMtx * rotateZMtx * rotateYMtx * rotateXMtx;
}

//=============================================================================
// Camera::SetClearColor
//=============================================================================
// 
// Description: Sets this camera's clear colour.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Camera::SetClearColor(float r, float g, float b) {
    m_ClearColor[0] = r;
    m_ClearColor[1] = g;
    m_ClearColor[2] = b;
}

//=============================================================================
// Camera::GetProjMatrix
//=============================================================================
// 
// Description: Constructs this camera's perspective projection matrix
// 
// Parameters:	N/A
// 
// Return:      [XMMATRIX]	This camera's perspective projection matrix
// 
//=============================================================================
XMMATRIX Camera::GetProjMatrix() {
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FieldOfView), (float)m_Width / m_Height, m_ZNear, m_ZFar);
}

//=============================================================================
// Camera::GetOrthoMatrix
//=============================================================================
// 
// Description: Constructs this camera's orthographic projection matrix.
//				Used primarily for drawing UI objects to the screen.
// 
// Parameters:	N/A
// 
// Return:      [XMMATRIX]	This camera's orthographic projection matrix
// 
//=============================================================================
XMMATRIX Camera::GetOrthoMatrix() {
    return XMMatrixOrthographicLH((float)m_Width, (float)m_Height, m_ZNear, m_ZFar);
}

//=============================================================================
// Camera::ScreenToWorldPoint
//=============================================================================
// 
// Description: Converts a specified point from screen-space to world-space,
//				relative to this camera's transformations.
// 
// Parameters:	[XMINT2]	Screen-space point to be converted
// 
// Return:      [XMFLOAT3]	World-space point
// 
//=============================================================================
XMFLOAT3 Camera::ScreenToWorldPoint(XMINT2 screenPos) {
    XMFLOAT3 cam_pos = m_Transform.GetPosition();

    XMFLOAT3 cam_right = m_Transform.GetRightVec();
    XMFLOAT3 cam_up = m_Transform.GetUpVec();
    XMFLOAT3 cam_forward = m_Transform.GetForwardVec();

    float Vnear = tanf(XMConvertToRadians(m_FieldOfView / 2));
    float Hnear = Vnear * ((float)m_Width / m_Height);

    XMFLOAT2 viewportPos = {
        screenPos.x / (m_Width / 2.0f) - 1.0f,
        screenPos.y / (m_Height / 2.0f) - 1.0f
    };

    XMFLOAT3 world_point = cam_pos + cam_forward + (cam_right * Hnear * viewportPos.x) - (cam_up * Vnear * viewportPos.y);

    return world_point;
}

//=============================================================================
// Camera::~Camera
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Camera::~Camera() {
    //
    // Free the framebuffer resources from memory
    //
    m_D3DBackBuffer->Release();
    m_D3DBackBufferView->Release();
    m_D3DBackBufferTexView->Release();

    m_D3DDepthBuffer->Release();
    m_D3DDepthBufferView->Release();
}
