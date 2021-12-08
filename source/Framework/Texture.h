//==============================================================================
// File: Texture.h
// 
// Description: Declares the Texture component.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <d3d11.h>

#include "GfxDevice.h"

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Texture {
public:
    Texture(GfxDevice* gfxDevice, const char* filepath);
    ~Texture();

    void Use();

private:
    GfxDevice* m_GfxDevice;

    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* textureView;

    int width, height;
    short bytes_per_pixel;
    int pitch;

    char* pixel_data;

    ID3D11SamplerState* samplerState;
};
