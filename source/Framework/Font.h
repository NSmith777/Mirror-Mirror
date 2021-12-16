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

class Font {
public:
    Font(GfxDevice* gfxDevice, FT_Library* pFt, const char* filepath, int size);
    ~Font();

    Character* GetMappedChar(int idx) { return &m_Characters[idx]; }
    ID3D11SamplerState* GetSamplerState() { return m_D3DSamplerState; }

private:
    GfxDevice* m_GfxDevice;

    std::map<char, Character> m_Characters;
    ID3D11SamplerState* m_D3DSamplerState;
};
