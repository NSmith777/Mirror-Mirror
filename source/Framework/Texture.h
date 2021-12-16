#pragma once

#include <windows.h>
#include <d3d11.h>

#include "GfxDevice.h"

class Texture {
public:
    Texture(GfxDevice* gfxDevice, const char* filepath);
    ~Texture();

    void Use();

private:
    GfxDevice* m_GfxDevice;

    ID3D11Texture2D* m_D3DTexture;
    ID3D11ShaderResourceView* m_D3DTextureView;

    ID3D11SamplerState* m_D3DSamplerState;

    int m_Width, m_Height;
    short m_BytesPerPixel;
    int m_Pitch;

    char* m_PixelData;
};
