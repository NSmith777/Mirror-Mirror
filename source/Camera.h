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

class Camera {
public:
	Camera(GfxDevice* gfxDevice, Shader* screenShader);
	~Camera();

	void Use();

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjMatrix() { return projMtx; }

	Shader* GetShader() { return m_ScreenShader; }
	Transform* GetTransform() { return &transform; }

	D3D11_VIEWPORT* GetViewport() { return &viewport; }
	ID3D11ShaderResourceView* GetRenderTargetTex() { return backBufferTexView; }

	void SetClearColor(float r, float g, float b);
	void SetProjMatrix(float fov, float aspect, float zNear, float zFar);

private:
	GfxDevice* m_GfxDevice;
	Shader* m_ScreenShader;

	XMMATRIX projMtx;
	Transform transform;

	FLOAT clearColor[4];
	D3D11_VIEWPORT viewport;

	// Camera framebuffer
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* backBufferView;
	ID3D11ShaderResourceView* backBufferTexView;

	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* depthBufferView;

	ID3D11DepthStencilState* depthStencilState;
};
