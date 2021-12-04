#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"

class Image {
public:
	Image(GfxDevice* gfxDevice, Texture* pTexture, Shader* pShader, XMFLOAT2 pos, XMFLOAT2 new_size);
	~Image();

	void Render(Camera* cam);

private:
	GfxDevice* m_GfxDevice;
	Texture* m_Texture;
	Shader* m_Shader;
	Constants constants;

	XMFLOAT2 position;
	XMFLOAT2 size;

	ID3D11Buffer* vertexBuffer;
	ID3D11RasterizerState* rasterizerState;
};
