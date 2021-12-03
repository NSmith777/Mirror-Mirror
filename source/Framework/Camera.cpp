#include "Camera.h"
#include "Math.h"

using namespace Math;

Camera::Camera(GfxDevice* gfxDevice, Shader *screenShader) {
	m_GfxDevice = gfxDevice;
	m_ScreenShader = screenShader;

	backBufferView = NULL;
	backBufferTexView = NULL;
	depthBufferView = NULL;

	// Set default camera dimensions in pixels
	width = 1280;
	height = 720;

	// Set clear colour to black
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f;
	clearColor[3] = 1.0f;

	// Set viewport to fit the entire screen
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = 1.0f;
	viewport.Height = 1.0f;

	// Set up default projection properties
	fieldOfView = 60.0f;
	zNear = 0.1f;
	zFar = 1000.0f;

	////////// Init Framebuffers //////////

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 4;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2DMS, textureDesc.Format);

	m_GfxDevice->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &backBuffer);
	m_GfxDevice->GetDevice()->CreateRenderTargetView(backBuffer, &renderTargetViewDesc, &backBufferView);

	CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(D3D11_SRV_DIMENSION_TEXTURE2DMS, textureDesc.Format);

	m_GfxDevice->GetDevice()->CreateShaderResourceView(backBuffer, &shaderResourceViewDesc, &backBufferTexView);

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	backBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS, depthBufferDesc.Format);

	m_GfxDevice->GetDevice()->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);
	m_GfxDevice->GetDevice()->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &depthBufferView);
}

void Camera::Use() {
	m_GfxDevice->GetDeviceContext()->ClearRenderTargetView(backBufferView, clearColor);
	m_GfxDevice->GetDeviceContext()->ClearDepthStencilView(depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_GfxDevice->GetDeviceContext()->OMSetRenderTargets(1, &backBufferView, depthBufferView);
}

XMMATRIX Camera::GetViewMatrix() {
	XMFLOAT3 position = transform.GetPosition();
	XMFLOAT3 rotation = transform.GetRotation();

	XMMATRIX translateMtx = XMMatrixTranslation(-position.x, -position.y, -position.z);
	XMMATRIX rotateZMtx = XMMatrixRotationNormal({ 0, 0, 1 }, -rotation.z);
	XMMATRIX rotateYMtx = XMMatrixRotationNormal({ 0, 1, 0 }, -rotation.y);
	XMMATRIX rotateXMtx = XMMatrixRotationNormal({ 1, 0, 0 }, -rotation.x);

	return translateMtx * rotateZMtx * rotateYMtx * rotateXMtx;
}

void Camera::SetClearColor(float r, float g, float b) {
	clearColor[0] = r;
	clearColor[1] = g;
	clearColor[2] = b;
}

XMMATRIX Camera::GetProjMatrix() {
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(fieldOfView), (float)width / height, zNear, zFar);
}

XMMATRIX Camera::GetOrthoMatrix() {
	return XMMatrixOrthographicLH((float)width, (float)height, zNear, zFar);
}

XMFLOAT3 Camera::ScreenToWorldPoint(XMINT2 screenPos) {
	XMFLOAT3 cam_pos = transform.GetPosition();

	XMFLOAT3 cam_right = transform.GetRightVec();
	XMFLOAT3 cam_up = transform.GetUpVec();
	XMFLOAT3 cam_forward = transform.GetForwardVec();

	float Vnear = tanf(XMConvertToRadians(fieldOfView / 2));
	float Hnear = Vnear * ((float)width / height);

	XMFLOAT2 viewportPos = {
		screenPos.x / (width / 2.0f) - 1.0f,
		screenPos.y / (height / 2.0f) - 1.0f
	};

	XMFLOAT3 world_point = cam_pos + cam_forward + (cam_right * Hnear * viewportPos.x) - (cam_up * Vnear * viewportPos.y);

	return world_point;
}

Camera::~Camera() {

}