#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "GfxDevice.h"

class Texture {
public:
	Texture(GfxDevice* gfxDevice);
	~Texture();

	void Use();

private:
	GfxDevice* m_GfxDevice;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureView;
};
