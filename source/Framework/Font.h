//==============================================================================
// File: Font.h
// 
// Description: Declares the Font component.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <map>

#include "GfxDevice.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace DirectX;

struct Character {
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* textureView;

    XMINT2 Size;
    XMINT2 Bearing;
    int Advance;
};

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Font {
public:
    Font(GfxDevice* gfxDevice, FT_Library* pFt, const char* filepath, int size);
    ~Font();

    Character* GetMappedChar(int idx) { return &Characters[idx]; }
    ID3D11SamplerState* GetSamplerState() { return samplerState; }

private:
    GfxDevice* m_GfxDevice;

    std::map<char, Character> Characters;
    ID3D11SamplerState* samplerState;
};
