#include "Camera.h"

Camera::Camera(GfxDevice* gfxDevice, Shader *screenShader) {
	m_GfxDevice = gfxDevice;
	m_ScreenShader = screenShader;

	backBufferView = NULL;
	backBufferTexView = NULL;
	depthBufferView = NULL;

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

	// Set up a default projection matrix
	projMtx = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 16.0f / 9, 0.1f, 1000.0f);

	////////// Init Framebuffers //////////

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 1280;
	textureDesc.Height = 720;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	m_GfxDevice->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &backBuffer);
	m_GfxDevice->GetDevice()->CreateRenderTargetView(backBuffer, nullptr, &backBufferView);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	m_GfxDevice->GetDevice()->CreateShaderResourceView(backBuffer, &shaderResourceViewDesc, &backBufferTexView);

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	backBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_GfxDevice->GetDevice()->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);
	m_GfxDevice->GetDevice()->CreateDepthStencilView(depthBuffer, nullptr, &depthBufferView);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	m_GfxDevice->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
}

void Camera::Use() {
	m_GfxDevice->GetDeviceContext()->ClearRenderTargetView(backBufferView, clearColor);
	m_GfxDevice->GetDeviceContext()->ClearDepthStencilView(depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_GfxDevice->GetDeviceContext()->OMSetRenderTargets(1, &backBufferView, depthBufferView);
	m_GfxDevice->GetDeviceContext()->OMSetDepthStencilState(depthStencilState, 0);
	m_GfxDevice->GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff); // use default blend mode (i.e. disable)
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

void Camera::SetProjMatrix(float fov, float aspect, float zNear, float zFar) {
	projMtx = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect, zNear, zFar);
}

Camera::~Camera() {

}
